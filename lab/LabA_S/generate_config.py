import os

# === 配置区域 ===
# 测试文件目录
TEST_CASE_DIR = r"test_case\bin"

# 基础输出目录
OUTPUT_TOML = "test_config.toml"

# 分割符数量
LABEL_NUM = 30
# ===============

def ensure_dir(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)

def generate_toml():
    if not os.path.exists(TEST_CASE_DIR):
        print(f"[Error] Directory {TEST_CASE_DIR} not found.")
        return

    # 扫描文件下所有的 ".bin" 文件
    files = [f for f in os.listdir(TEST_CASE_DIR) if f.endswith(".bin")]
    files.sort()

    if not files:
        print("No test files found.")
        return

    print(f"Found {len(files)} files. Generating {OUTPUT_TOML}...")
    index = 0

    with open(OUTPUT_TOML, "w", encoding="utf-8") as f:
        f.write("# " + "=" * LABEL_NUM + "\n")
        f.write("# LC-3 模拟器测试配置" + "\n")
        f.write("# " + "=" * LABEL_NUM + "\n\n")

        for filename in files:
            name = os.path.splitext(filename)[0]

            index += 1

            f.write("# " + "-" * LABEL_NUM + "\n")
            f.write(f"# Test{index}\n")
            f.write("# " + "-" * LABEL_NUM + "\n")

            f.write("[[case]]\n")
            f.write(f'name = "{name}"\n')
            f.write(f'file = "{filename}"\n')
            f.write(f'desc = "..."\n\n')

            # 生成初始状态模版
            f.write("\t[case.init]\n")
            f.write("\t# PC = 0x3000\n")
            f.write("\t# R0 = 10\n")
            f.write('\t# "M0x3000" = 0x1234\n\n')

            # 生成预期结果模版
            f.write("\t[case.expect]\n")
            f.write("\t# R0 = 15\n")
            f.write("\t# PC = 0x3005\n\n")

    print(f"Done! Saved to {OUTPUT_TOML}")

if __name__ == "__main__":
    generate_toml()
