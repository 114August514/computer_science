import os
import subprocess
import sys
# import filecmp    # 原本以为是两个二进制文件比较的

# === 配置区域 ===
# 获取项目根目录
PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# 汇编器可执行文件路径
ASSEMBLER_EXE = os.path.join(PROJECT_ROOT, "cmake-build-debug", "lc3_asm.exe")

# 测试样例目录
TEST_CASE_DIR = os.path.join(PROJECT_ROOT, "test_case", "asm")

# 预期输出目录
EXPECTED_DIR = os.path.join(PROJECT_ROOT, "test_case", "bin")

# 基础输出目录
OUTPUT_DIR = os.path.join(PROJECT_ROOT, "test_case", "output")
BIN_OUTPUT_DIR = os.path.join(OUTPUT_DIR, "bin")
SYMBOL_OUTPUT_DIR = os.path.join(OUTPUT_DIR, "symbol")
# ===============

def ensure_dir(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)

# 比较二进制文件和文本文件
def compare_bin_with_text(bin_path, text_path):
    with open(bin_path, 'rb') as f:
        raw_data = f.read()

    with open(text_path, 'r', encoding='utf-8') as f:
        expected_lines = [line.strip() for line in f.readlines() if line.strip()]

    # 将二进制转换成 01 字符串列表
    generated_lines = []
    for i in range(0, len(raw_data), 2):
        if i + 1 < len(raw_data):
            # 大端序
            word = (raw_data[i] << 8) | raw_data[i+1]
            generated_lines.append(f"{word:016b}")

    # 逐行比较
    if len(generated_lines) != len(expected_lines):
        return False, f"Line count mismatch: Generated {len(generated_lines)} lines vs Expected {len(expected_lines)} lines"
    
    for idx, (gen, exp) in enumerate(zip(generated_lines, expected_lines)):
        if gen != exp:
            return False, f"Mismatch at line {idx+1}: Gen '{gen}' vs Exp '{exp}'"

    return True, "Perfect Match"

def run_test_single(file_name):
    asm_path = os.path.join(TEST_CASE_DIR, file_name)
    if not file_name.endswith(".asm"):
        asm_path += ".asm"
        file_name += ".asm"

    expected_bin_name = file_name + ".bin"
    bin_name = file_name.replace(".asm", ".bin")
    out_path = os.path.join(BIN_OUTPUT_DIR, bin_name)

    print(f"[*] Testing {file_name} ...")

    # 1. 运行汇编器
    # 格式: assembler [options] <input.asm> <output.bin>
    cmd = [ASSEMBLER_EXE, "-d", asm_path, out_path]

    try:
        # 捕获标准输出以便出错时查看
        result = subprocess.run(cmd, capture_output=True, text=True)

        if result.returncode != 0:
            print("\n[FAILED] Assembler returned error.")
            print("--- Error ---")
            print(result.stdout)
            return False
        
        print("    [PASS] (Assembled)", end=" ")

        # 输出 Label 表格
        if SYMBOL_OUTPUT_DIR:
            stdout_content = result.stdout
            start_marker = "-- -Symbol Table-- -"
            end_marker = "--------------------"
            
            idx_start = stdout_content.find(start_marker)
            if idx_start != -1:
                idx_end = stdout_content.find(end_marker, idx_start)
                if idx_end != -1:
                    idx_end += len(end_marker)
                    sym_content = stdout_content[idx_start:idx_end]
                    
                    # 写入 symbol/xxxx.asm_symtable.txt
                    sym_file_name = file_name + "_symtable.txt"
                    sym_path = os.path.join(SYMBOL_OUTPUT_DIR, sym_file_name)
                    
                    with open(sym_path, "w", encoding="utf-8") as f:
                        f.write(sym_content)

        # 比较文件
        if EXPECTED_DIR:
            expected_path = os.path.join(EXPECTED_DIR, expected_bin_name)
            if os.path.exists(expected_path):
                match, msg = compare_bin_with_text(out_path, expected_path)

                if match:
                    print(f"    [MATCH] Binary matches expected output.")
                    return True
                else:
                    print(f"    [DIFF] {msg}")
                    return False
                
            else:
                print(f"    [WARN] Cannot find expected file.")
                return True
        return True
    
    except Exception as e:
        print(f"\n[ERROR] Execution failed: {e}")
        return False

def run_all():
    print(f"Scanning test cases in {TEST_CASE_DIR}...\n")
    files = [f for f in os.listdir(TEST_CASE_DIR) if f.endswith(".asm")]

    if not files:
        print("No .asm files found.")
        return
    
    passed = 0
    for f in files:
        if run_test_single(f):
            passed += 1

    print(f"\nSummary: {passed}/{len(files)} Passed.")

if __name__ == "__main__":
    ensure_dir(OUTPUT_DIR)
    ensure_dir(BIN_OUTPUT_DIR)
    ensure_dir(SYMBOL_OUTPUT_DIR)

    if not os.path.exists(ASSEMBLER_EXE):
        print(f"Error: Assembler executable not found at {ASSEMBLER_EXE}")
        sys.exit(1)

    # 命令行参数支持
    # python test_runner.py (运行所有)
    # python test_runner.py file_name (运行单个)
    if len(sys.argv) > 1:
        run_test_single(sys.argv[1])
    else:
        run_all()
