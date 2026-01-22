# [辅助脚本] 用于生成 TOML 模板文件

import os
from lc3test import env, utils

def generate_asm_config():
    """生成汇编器测试配置"""
    # 1. 确认路径
    asm_src_dir = env.ASM_SRC_DIR
    asm_expect_dir = env.EXPECT_BIN_DIR
    asm_symbal_dir = env.EXPECT_SYM_DIR
    config_path = os.path.join(env.CONFIG_DIR, "test_asm.toml")

    print(f"Scanning for Assembler tests in {asm_src_dir}...")

    if not os.path.exists(asm_src_dir):
        print(f"[Error] Dir not found: {asm_src_dir}")
        return

    # 2. 扫描文件
    files = [f for f in os.listdir(asm_src_dir) if f.endswith(".asm")]
    files.sort()

    if not files:
        print("[Error] No test files found.")

    # 3. 生成模板文件
    print(f"Found {len(files)} files. Generating {config_path}...")
    index = 0

    with open(config_path, "w", encoding="utf-8") as f:
        f.write("# " + "=" * env.LABEL_NUM + "\n")
        f.write("# LC-3 Assembler Config" + "\n")
        f.write("# " + "=" * env.LABEL_NUM + "\n\n")

        for asm_file in files:
            base_name = os.path.splitext(asm_file)[0]

            bin_expect_file = None
            sym_expect_file = None
            if os.path.exists(os.path.join(asm_expect_dir, asm_file + ".bin")):
                # 注意：TOML 里写相对路径，相对于 asm_expect_dir
                bin_expect_file = asm_file + ".bin"

            if os.path.exists(os.path.join(asm_symbal_dir, asm_file + "_symtable.txt")):
                sym_expect_file = asm_file + "_symtable.txt"

            index += 1

            f.write("# " + "-" * env.LABEL_NUM + "\n")
            f.write(f"# Test{index}\n")
            f.write("# " + "-" * env.LABEL_NUM + "\n")

            f.write(f"[[case]]\n")
            f.write(f'name = "{base_name}"\n')
            f.write(f'file = "{asm_file}"\n')  # 注意：TOML 里写相对路径，相对于 asm_src_dir
            f.write(f'desc = "..."\n\n')

            # 生成预期结果
            f.write("\t[case.expect]\n")
            if bin_expect_file:
                f.write(f'\tbin_expect = "{bin_expect_file}"\n')
            if sym_expect_file:
                f.write(f'\tsym_expect = "{sym_expect_file}"\n')

            f.write("\n\n")

    print(f"Done! Saved to {config_path}\n")


def generate_sim_config():
    """生成模拟器测试配置"""
    # 1. 确认路径
    sim_src_dir = env.EXPECT_BIN_DIR
    config_path = os.path.join(env.CONFIG_DIR, "test_sim.toml")

    print(f"Scanning for Simulator tests in {sim_src_dir}...")

    if not os.path.exists(sim_src_dir):
        print(f"[Error] Dir not found: {sim_src_dir}")
        return

    # 2. 扫描文件下所有的 ".bin" 文件
    files = [f for f in os.listdir(sim_src_dir) if f.endswith(".bin")]
    files.sort()

    if not files:
        print("[Error] No test files found.")
        return

    # 3. 生成模板文件
    print(f"Found {len(files)} files. Generating {config_path}...")
    index = 0

    with open(config_path, "w", encoding="utf-8") as f:
        f.write("# " + "=" * env.LABEL_NUM + "\n")
        f.write("# LC-3 Simulator Config" + "\n")
        f.write("# " + "=" * env.LABEL_NUM + "\n\n")

        for filename in files:
            name = os.path.splitext(filename)[0]

            index += 1

            f.write("# " + "-" * env.LABEL_NUM + "\n")
            f.write(f"# Test{index}\n")
            f.write("# " + "-" * env.LABEL_NUM + "\n")

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

    print(f"Done! Saved to {config_path}\n")


def generate_integ_config():
    """生成集成测试配置"""
    # 1. 确认路径
    src_dir = env.ASM_SRC_DIR
    config_path = os.path.join(env.CONFIG_DIR, "test_integ.toml")

    print(f"Scanning for Integration tests in {src_dir}...")

    if not os.path.exists(src_dir):
        print(f"[Error] Dir not found: {src_dir}")
        return

    # 2. 扫描文件
    files = [f for f in os.listdir(src_dir) if f.endswith(".asm")]
    files.sort()

    if not files:
        print("[Error] No test files found.")

    # 3. 生成模板文件
    print(f"Found {len(files)} files. Generating {config_path}...")
    index = 0

    with open(config_path, "w", encoding="utf-8") as f:
        f.write("# " + "=" * env.LABEL_NUM + "\n")
        f.write("# LC-3 Integration Config" + "\n")
        f.write("# " + "=" * env.LABEL_NUM + "\n\n")

        for filename in files:
            file_path = os.path.join(src_dir, filename)
            init_data, config_data, expect_data = utils.parse_magic_comments(file_path)

            base_name = os.path.splitext(filename)[0]
            index += 1

            f.write("# " + "-" * env.LABEL_NUM + "\n")
            f.write(f"# Test{index}\n")
            f.write("# " + "-" * env.LABEL_NUM + "\n")

            f.write(f"[[case]]\n")
            f.write(f'name = "{base_name}"\n')
            f.write(f'file = "{filename}"\n')  # 注意：TOML 里写相对路径，相对于 src_dir
            f.write(f'desc = "..."\n\n')

            # 生成初始状态模版
            f.write("\t[case.init]\n")
            if init_data:
                for key, val in init_data.items():
                    f.write(f"\t{key} = {val}\n")
                f.write("\n")
            else:
                # 默认模板
                f.write("\t# PC = 0x3000\n")
                f.write("\t# R0 = 10\n")
                f.write('\t# "M0x3000" = 0x1234\n\n')

            # 生成配置模版
            f.write("\t[case.config]\n")
            if config_data:
                for key, val in config_data.items():
                    f.write(f"\t{key} = {val}\n")
                f.write("\n")
            else:
                f.write("\t# stop_at = 0x3010\n\n")

            # 生成预期结果模版
            f.write("\t[case.expect]\n")
            if expect_data:
                for key, val in expect_data.items():
                    f.write(f"\t{key} = {val}\n")
                f.write("\n")
            else:
                f.write("\t# R0 = 15\n")
                f.write("\t# PC = 0x3005\n\n")

            f.write("\n")

    print(f"Done! Saved to {config_path}\n")


if __name__ == "__main__":
    utils.ensure_dir(env.CONFIG_DIR)

    # generate_asm_config()  # 需要自动化生成 asm 的测试配置文件时取消注释
    # generate_sim_config()   # 需要自动化生成 sim 的测试配置文件时取消注释
    # generate_integ_config()  # 需要自动化生成 integ 的测试配置文件时取消注释

    print("Done.")
