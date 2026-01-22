# 配置文件的类型标准

from dataclasses import dataclass, field
from abc import ABC, abstractmethod
from typing import Optional

# ===============================
# 1. 测试配置基类
# ===============================
@dataclass
class TestCase(ABC):
    name: str
    file: str
    desc: str = ""

    @classmethod
    @abstractmethod
    def from_toml_dict(cls, data: dict[str, any]) -> 'TestCase':
        """[接口定义] 工厂方法：从 .toml 格式的字典创建对象"""
        """子类必须实现"""
        raise NotImplementedError("Subclasses must implement from_toml_dict")

# ===============================
# 2. 汇编器测试配置类
# ===============================
@dataclass
class AsmCase(TestCase):
    bin_expect: Optional[str] = None
    sym_expect: Optional[str] = None

    @classmethod
    def from_toml_dict(cls, data: dict[str, any]) -> 'AsmCase':
        """工厂方法：从 .toml 格式的字典创建对象"""
        # a. 提取基础字段
        name: str = data.get("name", "Unnamed")
        file: str = data.get("file")
        desc: str = data.get("desc", "")

        if not file:
            raise ValueError(f"Case '{name}' missing 'file' field.")

        # b. 提取可选字段（如果仅运行测试汇编器，该字段必须有）
        expect_node: dict = data.get("expect")
        bin_expect = expect_node.get("bin_expect", None)
        sym_expect = expect_node.get("sym_expect", None)

        return cls(name=name, file=file, desc=desc, bin_expect=bin_expect, sym_expect=sym_expect)

# ===============================
# 3. 模拟器测试配置类
# ===============================
@dataclass
class SimCase(TestCase):
    init_state: dict[str, int] = field(default_factory=dict)
    expect_state: dict[str, int] = field(default_factory=dict)
    stop_at: Optional[int] = None

    @classmethod
    def from_toml_dict(cls, data: dict[str, any]) -> 'SimCase':
        """工厂方法：从 .toml 格式的字典创建对象"""
        # a. 提取基础字段
        name: str = data.get("name", "Unnamed")
        file: str = data.get("file")
        desc: str = data.get("desc", "")

        if not file:
            raise ValueError(f"Case '{name}' missing 'file' field.")

        # b. 提取配置
        config_node: dict = data.get("config", {})
        stop_at = config_node.get("stop_at")

        init_state = data.get("init", {})
        expect_state = data.get("expect", {})

        return cls(name=name, file=file, desc=desc, init_state=init_state, expect_state= expect_state, stop_at= stop_at)


# ===============================
# 4. 集成配置类（输入参考 AsmCase, 输出参考 SimCase）
# ===============================
@dataclass
class IntegrationCase(TestCase):
    init_state: dict[str, int] = field(default_factory=dict)
    expect_state: dict[str, int] = field(default_factory=dict)
    stop_at: Optional[int] = None

    @classmethod
    def from_toml_dict(cls, data: dict[str, any]) -> 'IntegrationCase':
        """工厂方法：从 .toml 格式的字典创建对象"""
        # a. 提取基础字段
        name: str = data.get("name", "Unnamed")
        file: str = data.get("file")
        desc: str = data.get("desc", "")

        if not file:
            raise ValueError(f"Case '{name}' missing 'file' field.")

        # b. 提取配置
        config_node: dict = data.get("config", {})
        stop_at = config_node.get("stop_at")

        init_state = data.get("init", {})
        expect_state = data.get("expect", {})

        return cls(name=name, file=file, desc=desc, init_state=init_state, expect_state= expect_state, stop_at= stop_at)
