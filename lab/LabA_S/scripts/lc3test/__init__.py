# ===============================
# Package Metadata (包元数据)
# ===============================
__version__ = "1.0.0"
__author__ = "114August514"

# ===============================
# 扁平化接口
# ===============================
# 1. 暴露环境常量
from .env import (
    ASM_EXE,
    SIM_EXE,
    CONFIG_DIR,
    BUILD_DIR
)

# 2. 暴露数据模型
from .models import (
    TestCase,
    AsmCase,
    SimCase,
    IntegrationCase
)

# 3. 暴露运行器
from .core import (
    BaseRunner,
    AssemblerRunner,
    SimulatorRunner,
    IntegrationRunner
)

# 4. 核心工具的外部接口
from .utils import (
    load_toml,
    Colors
)

# ===============================
# 导出控制(保护)
# 当外部执行 `from lc3test import *` 时，只允许导出以下内容
# ===============================

__all__ = [
    # 环境常量
    "ASM_EXE", "SIM_EXE", "CONFIG_DIR", "BUILD_DIR",

    # 数据模型
    "AsmCase", "SimCase", "IntegrationCase",

    # 运行器
    "AssemblerRunner", "SimulatorRunner", "IntegrationRunner", 

    # 工具
    "load_toml", "Colors"
]