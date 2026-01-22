# 用于运行器的调度

import os
import sys
import argparse
from typing import Optional
from lc3test import core, env, models, utils

# ===============================
# 0. 定义参数结构
# ===============================
class Arguments(argparse.Namespace):
    asm: bool
    sim: bool
    integ: bool
    all: bool
    filter: Optional[str]

# ===============================
# 1. 通用调度器
# ===============================
def run_suite(
        suite_name: str,                        # 调度器名称
        config_file: str,                       # TOML 配置文件路径
        ModelClass: type[models.TestCase],      # 使用的数据模型类
        RunnerClass: type[core.BaseRunner],     # 使用的执行器类
        filter: Optional[str]                   # 过滤器
    ) -> tuple[int, int]:                       # 输出：(passed_count, total_run_count)
    """通用的测试套件运行逻辑"""
    # 1. 加载配置文件
    if not os.path.exists(config_file):
        print(f"{utils.Colors.WARN}[Warning] Config not found: {config_file}{utils.Colors.ENDC}")
        return 0, 0

    raw_cases = utils.load_toml(config_file)
    if not raw_cases:
        return 0, 0

    print(f"\n{utils.Colors.INFO}=== Running {suite_name} ({len(raw_cases)}) ==={utils.Colors.ENDC}")

    # 开始准备并执行执行器
    passed_count = 0
    total_run_count = 0

    for case in raw_cases:
        name: str = case.get("name", "")
        file: str = case.get("file", "")

        # 2. 过滤逻辑（仅在传入该参数后进行筛选）
        if filter:
            key = filter.lower()
            if (key not in name.lower()) and (key not in file.lower()):
                continue

        # 3. 执行逻辑
        try:
            # A. 类型转换
            case_obj = ModelClass.from_toml_dict(case)

            # B. 实例化执行器
            runner = RunnerClass(case_obj)

            # C. 运行执行器
            if runner.run():
                passed_count += 1

            total_run_count += 1

        except Exception as e:
            print(f"{utils.Colors.FAIL}[Error] Case '{name}': {e}{utils.Colors.ENDC}")
            total_run_count += 1

    return passed_count, total_run_count

# ===============================
# 2. 主入口
# ===============================
def main() -> int:
    # a. 创建解析器
    parser = argparse.ArgumentParser(description="LC-3 System Test Suite")

    # b. 添加命令行参数
    # b.1 可选位置参数
    parser.add_argument("filter", nargs="?", help="Filter tests by name or filename")

    # b.2 可选参数(开关性质)
    parser.add_argument("--asm", action="store_true", help="Run Assembler Tests")
    parser.add_argument("--sim", action="store_true", help="Run Simulator Tests")
    parser.add_argument("--integ", action="store_true", help="Run Integration Tests")
    parser.add_argument("--all", action="store_true", help="Run All Tests")

    # c. 解析命令行参数并处理
    args = parser.parse_args(namespace=Arguments())

    # c.1 环境检查
    missing = []
    if not os.path.exists(env.ASM_EXE):
        missing.append(f"Assembler ({env.ASM_EXE})")
    if not os.path.exists(env.SIM_EXE):
        missing.append(f"Simulator ({env.SIM_EXE})")

    if missing:
        print(f"{utils.Colors.FAIL}[System Error] Executables not found:{utils.Colors.ENDC}")
        for miss in missing:
            print(f"  - {miss}")
        print("Please build the project first")
        return 1

    # c.2 处理无参数（默认情况）
    if not (args.asm or args.sim or args.integ):
        args.all = True

    total_pass = 0
    total_run = 0

    # c.3 调度汇编器测试
    if args.asm or args.all:
        passed, total = run_suite(
            suite_name = "Assembler Tests",
            config_file = os.path.join(env.CONFIG_DIR, "test_asm.toml"),
            ModelClass = models.AsmCase,
            RunnerClass = core.AssemblerRunner,
            filter = args.filter 
        )

        total_pass += passed
        total_run += total

    # c.4 调度模拟器测试
    if args.sim or args.all:
        passed, total = run_suite(
            suite_name="Simulator Tests",
            config_file=os.path.join(env.CONFIG_DIR, "test_sim.toml"),
            ModelClass=models.SimCase,
            RunnerClass=core.SimulatorRunner,
            filter=args.filter,
        )

        total_pass += passed
        total_run += total

    # c.5 调度集成测试
    if args.integ or args.all:
        passed, total = run_suite(
            suite_name="Integration Tests",
            config_file=os.path.join(env.CONFIG_DIR, "test_integ.toml"),
            ModelClass=models.IntegrationCase,
            RunnerClass=core.IntegrationRunner,
            filter=args.filter,
        )

        total_pass += passed
        total_run += total

    # d 评估与报告
    if total_run == 0:
        print("No tests found.")
        return 0
    
    print("-" * env.LABEL_NUM)
    
    summary_color = utils.Colors.PASS if total_pass == total_run else utils.Colors.FAIL
    print(f"Total Summary: {summary_color} {total_pass}/{total_run} Passed.{utils.Colors.ENDC}")

    return 0 if total_pass == total_run else 1

if __name__ == "__main__":
    sys.exit(main())
