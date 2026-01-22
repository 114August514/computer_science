# 通用工具

import os
import re
import sys
import struct
import tomllib

# ===============================
# 1. 终端美化类
# ===============================
class Colors:
    # A. 定义颜色代码常量
    _PASS = "\033[92m"  # 绿色
    _FAIL = "\033[91m"  # 红色
    _WARN = "\033[93m"  # 黄色
    _INFO = "\033[96m"  # 青色
    _ENDC = "\033[0m"   # 重置

    # B. 自动检查是否启用颜色
    # sys.stdout.isatty() 返回 True 表示连接的是终端
    # 并允许设置环境变量 NO_COLOR
    ENABLED = sys.stdout.isatty() and not os.getenv("NO_COLOR")

    # C. 根据检测结果对外暴露属性
    PASS = _PASS if ENABLED else ""
    FAIL = _FAIL if ENABLED else ""
    WARN = _WARN if ENABLED else ""
    INFO = _INFO if ENABLED else ""
    ENDC = _ENDC if ENABLED else ""

    @classmethod
    def force_disable(cls):
        # 手动禁用颜色的接口
        cls.PASS = ""
        cls.FAIL = ""
        cls.WARN = ""
        cls.INFO = ""
        cls.ENDC = ""

# ===============================
# 2. 文件相关操作
# ===============================
def ensure_dir(dir: str):
    """确保目录存在"""
    if not os.path.exists(dir):
        os.makedirs(dir)

def load_toml(path: str) -> list[dict]:
    """加载 toml 配置文件，并提取 case 列表"""
    if not os.path.exists(path):
        return {}

    try:
        with open(path, "rb") as f:
            data = tomllib.load(f)
            return data.get("case", [])
        
    except Exception:
        return []

# ===============================
# 3. 转换文件格式
# ===============================
def is_text_01(file_path: str) -> bool:
    """检测文件是否为包含 '01' 串的文本文件"""
    if not os.path.exists(file_path):
        return False

    try:
        with open(file_path, "rb") as f:
            # 读取 8 个字节判断文件类型
            header = f.read(8)

        if not header:
            return False

        # 定义允许的非注释字符
        valid_chars = b"01 \n\r\t"
        SEMICOLON = ord(';')
        NEWLINE = ord('\n')

        # 是否在注释里
        in_comment = False

        for byte in header:
            # 1. 如果在注释里
            if in_comment:
                if byte == NEWLINE:
                    in_comment = False
                continue

            # 2. 如果遇到 ';'
            if byte == SEMICOLON:
                in_comment = True
                continue

            # 3. 检查有效字符
            if byte not in valid_chars:
                return False

        return True

    except Exception:
        return False

def text_01_to_bin(text_path: str, bin_path: str) -> bool:
    """将写作 '01' 串的文本文件转换成二进制文件"""
    try:
        with open(text_path, "r", encoding="utf-8") as f_in:
            lines = [line.strip() for line in f_in.readlines() if line.strip()]

        with open(bin_path, "wb") as f_out:
            for line in lines:
                # 去除注释
                line = line.split(";")[0].strip()
                if not line:
                    continue

                # 保证 16 bits
                if len(line) != 16:
                    return False

                val = int(line, 2)  # 将字符串按 2 进制转换为整数

                # 用 struct 模块将 val 值按照大端序打包
                # > : 大端序
                # H : 2 字节
                f_out.write(struct.pack(">H", val))
        return True
    
    except Exception:
        return False

def bin_to_text_01(bin_path: str) -> list[str]:
    """将二进制文件读取为 '01...' 字符串列表"""
    try:
        with open(bin_path, "rb") as f:
            raw_data = f.read()

        # 确保数据长度是偶数（2字节对齐）
        if len(raw_data) % 2 != 0:
            return []

        # '>H' 表示大端序的16位无符号整数
        values = struct.unpack(f">{len(raw_data)//2}H", raw_data)

        # 格式化为16位二进制字符串
        return [f"{val:016b}" for val in values]

    except Exception:
        return []

# ===============================
# 4. 比对函数
# ===============================
def compare_bin_with_text(bin_path: str, text_path: str) -> tuple[bool, str]:
    """
    比对生成的二进制文件与预期的文本文件。
    返回：(是否通过, 错误信息)
    """
    try:
        # 1. 获取二进制内容的字符串形式
        gen_lines = bin_to_text_01(bin_path)

        # 2. 获取期望的数据
        with open(text_path, "r", encoding="utf-8") as f:
            expected_lines = [line.strip() for line in f.readlines() if line.strip()]

        # 3. 逐行比较
        if len(gen_lines) != len(expected_lines):
            return False, f"Line count mismatch: Generated {len(gen_lines)} lines vs Expected {len(expected_lines)} lines"

        for idx, (gen, exp) in enumerate(zip(gen_lines, expected_lines)):
            if gen != exp:
                return False, f"Mismatch at line {idx+1}: Got '{gen}' vs Exp '{exp}'"

        return True, "Match"

    except Exception as e:
        return False, f"Exception: {str(e)}"

def verify_sim_state(actual_state: dict[str, int], expect_state: dict[str, int]) -> list[str]:
    """比较模拟器实际运行结果与预期结果"""
    failed_reasons = []
    for key, exp_val in expect_state.items():
        act_val = actual_state.get(key.upper(), None)

        if act_val == None:
            failed_reasons.append(f"{key} not found in output")
            continue

        if key.upper() == "COUNT":
            if act_val != exp_val:
                failed_reasons.append(f"{key}: Got {act_val}, Exp {exp_val}")
            continue

        if (act_val & 0xFFFF) != (exp_val & 0xFFFF):
            failed_reasons.append(f"{key}: Got 0x{act_val:04X}, Exp 0x{exp_val:04X}")

    return failed_reasons

# ===============================
# 5. 解析输出字符串函数
# ===============================
def parse_sim_output(output_text: str) -> dict[str, int]:
    """解析模拟器标准输出，提取寄存器和统计信息。"""
    state: dict[str, int] = {}

    # 1. 解析通用寄存器
    # 匹配格式：R<数字><可选空格>[:=]<可选空格>[0x | x]<十六进制数>
    reg_matches = re.findall(r"R([0-7])\s*[:=]\s*(?:0x|x)?([0-9a-fA-F]{1,4})", output_text)
    for reg_id, hex_val in reg_matches:
        state[f"R{reg_id}"] = int(hex_val, 16)

    # 2. 解析专用寄存器
    for key in ["PC", "IR"]:
        match = re.search(rf"{key}\s*[:=]\s*(?:0x|x)?([0-9a-fA-F]{1,4})", output_text)
        if match:
            state[key] = int(match.group(1), 16)

    # 3. 匹配 Instr Count
    count_match = re.search(r"Instr Count\s*[:=]\s*(\d+)", output_text)
    if count_match:
        state["COUNT"] = int(count_match.group(1))

    return state

# ===============================
# 6. 辅助配置文件生成函数
# ===============================
def _parse_key_val_string(raw_str: str) -> dict[str, any]:
    """辅助解析原始字符串变成键值对"""
    result = {}

    pattern = re.compile(r"([A-Za-z0-9_]+)\s*[:=]\s*([^,]+)")
    matches = pattern.findall(raw_str)

    for key, val in matches:
        key: str = key.strip().upper()
        val: str = val.strip()

        # 数值转换
        try:
            # A. 十六进制 (x3000 或 0x3000)
            if val.lower().startswith('x'):
                val_int = int(val[1:], 16)
                result[key] = f"0x{val_int:X}"

            elif val.lower().startswith("0x"):
                val_int = int(val, 16)
                result[key] = f"0x{val_int:X}"

            # B. 十进制 (#10 或 10)
            elif val.startswith('#'):
                result[key] = int(val[1:])

            else:
                result[key] = int(val)

        except ValueError:
            # 如果不是数字
            result[key] = f'"{val}"'

    return result

def parse_magic_comments(src_path: str) -> tuple[dict[str, any], dict[str, any], dict[str, any]]:
    """
    [用法]解析 .asm 文件中辅助配置文件生成的特殊注释。

    格式：
        ; @INIT: R0 = 10, PC = x3000
        ; @CONFIG: stop_at = 0x3010
        ; @EXPECT: R1 = xFFFF, COUNT = 10
    """
    init_data = {}
    expect_data = {}
    config_data = {}

    try:
        with open(src_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()

        # 正则匹配关键字
        for line in content.splitlines():
            line = line.strip()

            if not line.startswith(';'):
                continue

            # 解析 @INIT
            if "@INIT:" in line:
                raw = line.split("@INIT:")[1].strip()
                init_data.update(_parse_key_val_string(raw))

            # 解析 @CONFIG
            if "@CONFIG:" in line:
                raw = line.split("@CONFIG:")[1].strip()
                config_data.update(_parse_key_val_string(raw))

            # 解析 @EXPECT
            if "@EXPECT:" in line:
                raw = line.split("@EXPECT:")[1].strip()
                expect_data.update(_parse_key_val_string(raw))

    except Exception as e:
        print(f"  [Warn] Failed to parse magic comments in {os.path.basename(src_path)}: {e}")

    return init_data, config_data, expect_data

