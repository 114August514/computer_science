# 环境配置

import os

# 定位项目根目录
ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))

# 定位构建目录
BUILD_DIR = os.path.join(ROOT_DIR, "cmake-build-debug")
if not os.path.exists(BUILD_DIR):
    BUILD_DIR = os.path.join(ROOT_DIR, "build")

# 可执行文件路径
ASM_EXE = os.path.join(BUILD_DIR, "lc3_asm.exe")
SIM_EXE = os.path.join(BUILD_DIR, "lc3_sim.exe")

# 数据目录
TEST_CASE_DIR = os.path.join(ROOT_DIR, "test_case")
CONFIG_DIR = os.path.join(ROOT_DIR, "config")
ASM_SRC_DIR = os.path.join(TEST_CASE_DIR, "asm")
EXPECT_BIN_DIR = os.path.join(TEST_CASE_DIR, "bin")
EXPECT_SYM_DIR = os.path.join(TEST_CASE_DIR, "symbol")

# 输出目录
OUTPUT_DIR = os.path.join(TEST_CASE_DIR, "output")
TEMP_DIR = os.path.join(TEST_CASE_DIR, "temp")

# 分隔符数量
LABEL_NUM = 30