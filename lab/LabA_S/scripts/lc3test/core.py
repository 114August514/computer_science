# 测试运行器类

import os
import time
import subprocess
from typing import Optional
from abc import ABC, abstractmethod

# 引入当前包下的辅助文件
from . import env, utils, models

# ===============================
# 1. 运行器基类
# ===============================
class BaseRunner(ABC):
    """测试运行器基类"""
    def __init__(self, case: models.TestCase):
        self.case = case
        self.name = case.name or os.path.basename(case.file)
        self.desc = case.desc

        # 临时文件注册表
        self._temp_files = []

    def _log(self, success: bool, msg: str = "", duration: float = 0.0) -> bool:
        """统一格式化输出"""
        status_color = utils.Colors.PASS if success else utils.Colors.FAIL
        status_text = "PASS" if success else "FAIL"

        # 获取当前类名
        runner_type = self.__class__.__name__[:3].upper()

        # 获取运行时间信息
        time_info = f"({duration:.1f}ms)" if duration > 0 else ""

        # 输出格式：[*] {RunnerType}: TestName [Status] (Time) (Message)
        print(f"[*] {runner_type}: {self.name:<25} {status_color}[{status_text}]{utils.Colors.ENDC} {time_info} {msg}")
        return success

    def _run_subprocess(self, cmd: list[str], timeout=5) -> tuple[Optional[subprocess.CompletedProcess], float]:
        """子进程调用与异常处理"""
        start_time = time.time()
        try:
            result = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout)
            duration = (time.time() - start_time) * 1000
            return result, duration

        except subprocess.TimeoutExpired:
            return None, (time.time() - start_time) * 1000

        except Exception as e:
            # 该异常一般是找不到可执行文件这种异常
            print(f"{utils.Colors.FAIL}[System Error] {e}{utils.Colors.ENDC}")
            return None, 0

    def register_temp_file(self, path: str):
        """注册需要清理的临时文件"""
        if path and path not in self._temp_files:
            self._temp_files.append(path)

    def clean_up(self):
        """清理所有注册的临时文件"""
        failed_files = []

        for file in self._temp_files:
            if os.path.exists(file):
                try:
                    os.remove(file)
                except OSError:
                    failed_files.append(file)

        if failed_files:
            self._log(False, f"Couldn't remove files: {failed_files}", 0)

        self._temp_files.clear()

    @abstractmethod
    def run(self) -> bool:
        """[接口定义] 执行测试流程"""
        raise NotImplementedError("Subclasses must implement run()")

# ===============================
# 2. 汇编器类
# ===============================
class AssemblerRunner(BaseRunner):
    def __init__(self, case: models.AsmCase):
        super().__init__(case)
        self.case = case # 确保 IDE 识别到 case 的类型

    def run(self) -> bool:
        """运行 lc3_asm 并验证汇编器正确性"""
        # 1. 准备文件路径
        src_file_path = os.path.join(env.ASM_SRC_DIR, self.case.file)

        out_file_name = os.path.splitext(os.path.basename(self.case.file))[0]
        out_file_path = os.path.join(env.OUTPUT_DIR, "bin", out_file_name + ".bin")
        symbol_out_path = os.path.join(env.OUTPUT_DIR, "symbol", out_file_name + "_symtable.txt")

        bin_expect_path = None
        sym_expect_path = None
        if self.case.bin_expect:
            bin_expect_path = os.path.join(env.EXPECT_BIN_DIR, self.case.bin_expect)
        if self.case.sym_expect:
            sym_expect_path = os.path.join(env.EXPECT_SYM_DIR, self.case.sym_expect)

        # 2. 执行汇编器
        success, msg, duration = self.compile(src_file_path, out_file_path, symbol_out_path)
        if not success:
            return self._log(False, msg, duration)

        # 3. 验证结果
        if not bin_expect_path:
            return self._log(True, "(Compile Only)", duration)

        if not os.path.exists(bin_expect_path):
            return self._log(False, f"Expect bin missing: {self.case.bin_expect}", duration)

        ok, compare_msg = utils.compare_bin_with_text(out_file_path, bin_expect_path)

        if not ok:
            return self._log(False, compare_msg, duration)

        if not sym_expect_path:
            return self._log(True, "(Match Bin Only)", duration)

        if not os.path.exists(sym_expect_path):
            return self._log(False, f"Expect symboltable missing: {self.case.sym_expect}", duration)

        ok, compare_msg = utils.compare_label_table(symbol_out_path, sym_expect_path)
        return self._log(ok, compare_msg, duration)

    def compile(self, src_path: str, out_path: str, symbol_out_path: Optional[str] = None) -> tuple[bool, str, float]:
        """仅调用 lc3_asm，进行编译操作"""
        if not os.path.exists(src_path):
            return False, f"Source not found: {src_path}", 0

        # 前期准备
        utils.ensure_dir(os.path.dirname(out_path))
        cmd = [env.ASM_EXE, src_path, out_path] # 不开启 debug 模式
        if symbol_out_path:
            utils.ensure_dir(os.path.dirname(symbol_out_path))
            cmd.append("-d")

        # 运行程序
        result, duration = self._run_subprocess(cmd)

        if result is None:
            return False, "Compile Error", duration

        if result.returncode != 0:
            err_head = result.stdout.split('\n') if result.stdout else "Unknown Error"
            return False, f"Compile Error: {err_head}", duration

        if not os.path.exists(out_path):
            return False, "Output Missing", duration

        # 处理需要输出符号表的逻辑
        if symbol_out_path:
            symbol_content = utils.parse_asm_output(result.stdout)
            if symbol_content:
                with open(symbol_out_path, "w", encoding="utf-8") as f:
                    f.write(symbol_content)

        # 当前步骤正确，回答由更高级做答
        return True, "", duration

# ===============================
# 3. 模拟器类
# ===============================
class SimulatorRunner(BaseRunner):
    def __init__(self, case: models.SimCase):
        super().__init__(case)
        self.case = case  # 确保 IDE 识别到 case 的类型

    def run(self) -> bool:
        """运行 lc3_sim 并验证模拟器正确性"""
        # 1. 准备文件路径
        src_file_path = os.path.join(env.EXPECT_BIN_DIR, self.case.file)
        run_path = src_file_path
        temp_bin = None

        try:
            # 2. (保证文件格式兼容)
            if utils.is_text_01(src_file_path):
                utils.ensure_dir(env.TEMP_DIR)
                temp_bin = os.path.join(env.TEMP_DIR, f"temp_{os.getpid()}_{self.name}.obj")

                self.register_temp_file(temp_bin)
                if not utils.text_01_to_bin(src_file_path, temp_bin):
                    return self._log(False, "Convert Error", 0)

                run_path = temp_bin

            # 3. 执行汇编器
            actual_state, success, msg, duration = self.execute(run_path)
            if not success:
                return self._log(False, msg, duration)

            # 4. 验证结果
            failed_reasons = utils.verify_sim_state(actual_state, self.case.expect_state)

            if not failed_reasons:
                return self._log(True, "Match", duration)

            error_msg = "\n".join(f"    - {reason}" for reason in failed_reasons)
            return self._log(False, error_msg, duration)

        finally:
            self.clean_up()

    def execute(self, run_path: str) -> tuple[dict[str, int], bool, str, float]:
        """仅调用 lc3_sim，执行模拟器，并返回解析后的状态"""
        # 1. 构造命令行参数
        cmd = [env.SIM_EXE, run_path]

        # 解析 [init] 部分
        for key, val in self.case.init_state.items():
            key_upper = key.upper()
            val_str = str(val)

            if key_upper == "PC":
                cmd.extend(["-pc", val_str])

            elif key_upper.startswith("R"):
                # 将 "R0" -> 0
                cmd.extend(["-reg", key_upper[1:], val_str])

            elif key_upper.startswith("M"):
                # 将 "M0x3000" -> 0x3000
                cmd.extend(["-mem", key_upper[1:], val_str])

        # 解析 [config] 部分
        if self.case.stop_at is not None:
            cmd.extend(["-break", str(self.case.stop_at)])

        # 2. 执行模拟器
        result, duration = self._run_subprocess(cmd)

        if result is None:
            return None, False, "Execute Error", duration

        if result.returncode != 0:
            err_head = result.stdout.split('\n') if result.stdout else "Unknown Error"
            return None, False, f"Execute Error: {err_head}", duration

        # 3. 解析参数
        actual_state = utils.parse_sim_output(result.stdout)
        # 当前步骤正确，回答由更高级做答
        return actual_state, True, "", duration

# ===============================
# 4. 集成运行器类
# ===============================
class IntegrationRunner(BaseRunner):
    def __init__(self, case: models.IntegrationCase):
        super().__init__(case)
        self.case = case

    def run(self) -> bool:
        # 1. 准备文件路径
        src_file_path = os.path.join(env.ASM_SRC_DIR, self.case.file)
        if not os.path.exists(src_file_path):
            return self._log(False, f"Source not found: {self.case.file}", 0)

        # 临时文件
        utils.ensure_dir(env.TEMP_DIR)
        temp_bin = os.path.join(env.TEMP_DIR, f"temp_{os.getpid()}_{self.name}.bin")
        self.register_temp_file(temp_bin)

        try:
            # 2. 用 Asm 汇编
            asm_helper = AssemblerRunner(self.case)
            asm_success, asm_msg, asm_duration = asm_helper.compile(src_file_path, temp_bin)
            if not asm_success:
                return self._log(False, asm_msg, asm_duration)

            # 3. 用 Sim 模拟
            sim_helper = SimulatorRunner(self.case)
            actual_state, sim_success, sim_msg, sim_duration = sim_helper.execute(temp_bin)
            duration = asm_duration + sim_duration
            if not sim_success:
                return self._log(False, sim_msg, duration)
            
            # 4. 验证结果
            failed_reasons = utils.verify_sim_state(actual_state, self.case.expect_state)

            if not failed_reasons:
                return self._log(True, "Match", duration)

            error_msg = "\n".join(f"    - {reason}" for reason in failed_reasons)
            return self._log(False, error_msg, duration)

        finally:
            self.clean_up()
