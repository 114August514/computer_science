import os
import re
import sys
import struct
import tomllib
import subprocess
from typing import Any, TypeAlias, TypedDict

# === 配置区域 ===
# 汇编器可执行文件路径
SIMULATOR_EXE : str = r"cmake-build-debug\lc3_sim.exe"

# 测试样例目录
TEST_CASE_DIR : str = r"test_case\bin"

# 配置文件路径
CONFIG_FILE : str = "test_config.toml"

# 临时二进制文件 (用于转换 文本文件 -> .bin)
TEMP_BIN : str = "temp_sim_test.bin"

# 基础输出目录
OUTPUT_DIR : str = r"test_case\output"
SIM_OUTPUT_DIR : str = os.path.join(OUTPUT_DIR, "sim")
# =======================

# === 类型设置 ===
Matches : TypeAlias = list[tuple[str, str]]

class Case(TypedDict, total=False):
    name: str
    file: str
    init: dict[str, int]
    config: dict[str, int]
    expect: dict[str, int]
# =======================

def ensure_dir(directory : str) -> None:
    if not os.path.exists(directory):
        os.makedirs(directory)

def text_to_bin(text_path : str, bin_path : str) -> bool: 
    # 将文本文件转换成二进制文件
    try:
        with open(text_path, "r", encoding='utf-8') as f:
            lines : list[str] = [line.strip() for line in f.readlines() if line.strip()]

        with open(bin_path, "wb") as f:
            for line in lines:
                line = line.split(';')[0].strip()
                if not line:
                    continue

                # 保证 16 bits
                if (len(line) != 16):
                    return False

                val = int(line, 2)  # 将字符串按 2 进制转换为整数

                # 用 struct 模块将 val 值按照大端序打包
                # > : 大端序
                # H : 2 字节
                f.write(struct.pack(">H", val))

        return True
    except Exception as e:
        print(f"[Convert Error] {e}")
        return False

def parse_output(output_text : str) -> dict[str, int]:
    # 解析输出
    state : dict[str, int] = {}

    # 1. 解析通用寄存器
    # 匹配格式：R<数字>: <可选空格> x<十六进制数>
    reg_matches : Matches = re.findall(r"R(\d+):\s*x([0-9A-Fa-f]{1,4})", output_text)
    for reg_id, hex_val in reg_matches:
        state[f"R{reg_id}"] = int(hex_val, 16)

    # 2. 解析专用寄存器
    pc_match = re.search(r"PC:\s*x([0-9A-Fa-f]+)", output_text)
    if pc_match:
        state["PC"] = int(pc_match.group(1), 16)

    ir_match = re.search(r"IR:\s*x([0-9A-Fa-f]+)", output_text)
    if ir_match:
        state["IR"] = int(ir_match.group(1), 16)

    count_match = re.search(r"Instr Count:\s*(\d+)", output_text)
    if count_match:
        state["COUNT"] = int(count_match.group(1))

    return state

def run_test_single(case : Case) -> bool:
    # 针对单个文件的测试
    test_name = case.get("name", "Unnamed")
    print(f"[*] Testing: {test_name:<20}")

    # 1. 找到源文件
    file_name = case.get("file", "")
    src_path = os.path.join(TEST_CASE_DIR, file_name)
    run_path = src_path # 默认直接运行（但要防 .bin 后缀的文本文件）

    if not os.path.exists(src_path):
        print(f"File Not Found: {file_name}.")
        return False

    # 2. 转换文件格式为二进制文件（主要针对的是 .bin 但文本文件）
    # 不接受格式错误的文件
    is_text = False
    try:
        with open(src_path, "rb") as f:
            header = f.read(8)

            if all(b in b"01 \n\r\t" for b in header):
                is_text = True
    except:
        pass

    if is_text:
        if not text_to_bin(src_path, TEMP_BIN):
            return False
        run_path = TEMP_BIN

    # 3. 构造命令行参数
    cmd : list[str] = [SIMULATOR_EXE, run_path]

    # 解析 [init] 部分
    init_data = case.get("init", {})
    for key, val in init_data.items():
        val_str = str(val)

        if key.upper() == "PC":
            cmd.extend(["-pc", val_str])

        elif key.upper().startswith("R"):
            reg_id = key[1:]
            cmd.extend(["-reg", reg_id, val_str])

        elif key.upper().startswith("M"):
            # 将 "M0x3000" -> 0x3000
            addr_str = key[1:]
            cmd.extend(["-mem", addr_str, val_str])

    # 解析 [config] 部分
    config_data = case.get("config", {})
    for key, val in config_data.items():
        val_str = str(val)

        if key.lower() == "stop_at":
            cmd.extend(["-break", val_str])

    # 4. 执行模拟器
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=2)
    except subprocess.TimeoutExpired:
        print(f"[TIMEOUT]")
        return False

    if result.returncode != 0:
        print("\n[FAILED] Simulator returned error.")
        print("--- Error ---")
        print(result.stdout)
        return False

    # 5. 解析结果与比对
    actual_state = parse_output(result.stdout)
    expect_data = case.get("expect", {})
    failed_reasons = []

    for key, exp_val in expect_data.items():
        act_val = actual_state.get(key.upper(), None)

        if act_val == None:
            failed_reasons.append(f"{key} not found in output")
            continue

        if key.upper() == "COUNT":
            if act_val != exp_val:
                failed_reasons.append(f"{key}: Exp {exp_val}, Got {act_val}")
            continue

        if (act_val & 0xFFFF) != (exp_val & 0xFFFF):
            failed_reasons.append(f"{key}: Exp 0x{exp_val:04X}, Got 0x{act_val:04X}")

    # 6. 输出结论
    if not failed_reasons:
        print("[PASS]")
        return True
    
    print("[FAIL]")
    for reason in failed_reasons:
        print(f"    - {reason}")
    return False

if __name__ == "__main__":

    # 检查环境
    ensure_dir(OUTPUT_DIR)
    ensure_dir(SIM_OUTPUT_DIR)

    if not os.path.exists(SIMULATOR_EXE):
        print(f"Error: Simulator executable not found at {SIMULATOR_EXE}")
        sys.exit(1)

    if not os.path.exists(CONFIG_FILE):
        print(f"Error: Config file {CONFIG_FILE} not found.")
        sys.exit(1)

    # 加载配置
    with open(CONFIG_FILE, "rb") as f:
        config = tomllib.load(f)

    test_cases = config.get("case", [])
    if not test_cases:
        print("No test cases found in config.")
        sys.exit(0)

    print(f"Loaded {len(test_cases)} tests from {CONFIG_FILE}.\n")

    passed = 0
    for case in test_cases:
        if run_test_single(case):
            passed += 1
    
    print("-" * 40)
    print(f"Summary: {passed}/{len(test_cases)} Passed.")

    # 清理临时文件
    if os.path.exists(TEMP_BIN):
        os.remove(TEMP_BIN)
