# LC-3 System

![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg?logo=c%2B%2B) ![Build](https://img.shields.io/badge/Build-CMake-green.svg?logo=cmake) ![Test](https://img.shields.io/badge/Test-Python_3.11+-yellow.svg?logo=python) ![License](https://img.shields.io/badge/License-MIT-lightgrey.svg)

## 📖 项目简介 (Introduction)

本项目是一个完全遵循 **LC-3 (Little Computer 3)** 架构规范的计算机系统仿真工具链，采用 **Modern C++ (C++17)** 编写核心逻辑，并结合 **Python** 实现了工程化的自动化测试框架，旨在帮助开发者从底层理解计算机系统的运作机制。

项目不仅仅是简单的作业实现，更采用了工业级的软件工程实践：

* **模块化构建**：基于 Target 的 Modern CMake 架构。
* **双模式仿真**：支持标准 `.bin` 格式与自定义的 **高级多段稀疏格式**。
* **配置驱动测试**：基于 TOML 和 Python `dataclasses` 的自动化评测系统。

---

## 📂 目录结构 (Directory Structure)

``` text
LC3-System/
├── CMakeLists.txt       # CMake 构建配置文件
├── config/              # TOML 测试配置文件(均可通过 gen_config.py 生成模板)
│   ├── test_asm.toml    # 汇编器测试用例
│   ├── test_sim.toml    # 模拟器测试用例
│   └── test_integ.toml  # 集成测试配置 
├── include/             # 头文件目录
│   ├── assembler/       # 汇编器头文件
│   │   └── assembler.h
│   ├── common/          # 通用类型和工具
│   │   ├── types.h      # 数据类型定义
│   │   └── utils.h      # 工具函数
│   └── simulator/       # 模拟器头文件
│       └── lc3_vm.h
├── src/                 # 源代码实现
│   ├── assembler/       # 汇编器库实现
│   │   └── assembler.cpp
│   ├── simulator/       # 模拟器库实现
│   │   └── lc3_vm.cpp
│   ├── main_asm.cpp     # 汇编器 CLI 入口
│   └── main_sim.cpp     # 模拟器 CLI 入口
├── scripts/             # 测试器相关脚本
│   ├── run.py           # [统一入口] 测试调度器
│   ├── gen_config.py    # [工具] 配置生成脚本
│   └── lc3test/         # [核心包] 包含 Runners(core.py 中), Models, Utils
├── test_case/           # 测试用例
│   ├── asm/             # 汇编源码(.asm)
│   ├── bin/             # 标准答案(.bin)
│   ├── symbol/          # 符号表
│   └── lab/             # 实验参考代码
└── report/              # 实验报告
```

---

## ⚙️ 功能特性 (Features)

> 汇编器和模拟器的入口都有如何使用的说明。

### 🔧 汇编器 (Assembler)

* **Two-Pass Algorithm**: 第一遍建立符号表，第二遍生成机器码。
* **Smart Output**:
  * **Standard Mode**: 生成兼容官方工具的 `.bin` 文件。
  * **Advanced Mode**: 通过 Magic Number (`0x1C3B`) 识别，支持 **多段 .ORIG 块** 的稀疏存储，无需填充零，大幅减小文件体积。
* **Debug Info**: 编译时自动导出符号表 (`.sym.txt`)，辅助调试。

### 💻 模拟器 (Simulator)

* **Dual-Mode Loading**: 自动识别标准格式与高级多段格式二进制文件。
* **Interactive Debugger**: 内置交互式调试器，在 debug 模式下启用。
* **OJ Integration Mode**: 支持通过命令行 (-reg, -mem, -pc) 初始化状态，并输出机器可读的运行结果（Rx=0x...），方便自动化评测。

### 🧪 自动化测试框架 (Test Framework)

* **Modular Design**: 封装于 scripts/lc3test 包中，实现了 AssemblerRunner 和 SimulatorRunner 的解耦与复用。
* **Config Driven**: 所有测试用例通过 TOML 文件管理。
* **Magic Comments**: 支持在 .asm 源码中直接定义测试断言，实现“代码即测试”（该功能目前仅针对 integ_cases）。

---

## 🔨 构建指南 (Build)

### 前置要求

* **C++ Compiler**: GCC/Clang/MSVC (支持 C++17)
* **CMake**: 3.15+
* **Python**: 3.11+ (低版本需安装 `tomli`)

### 编译步骤

```bash
# 创建构建目录
mkdir build && cd build

# 生成构建文件
cmake ..

# 编译
# make          # Linux 用户使用
cmake --build . # Windows 用户使用

# 虽然本人用的是 clion 来帮助直接编译的
```

构建完成后，将在 `cmake-build-debug/`(或 `build/`) 目录下生成：

* `lc3_asm`: 汇编器可执行文件
* `lc3_sim`: 模拟器可执行文件

---

## 🚀 使用说明 (Usage)

### 1. 汇编器 (lc3_asm)

```bash
# 基本用法
lc3_asm <input.asm> <output.bin>

# 调试模式（输出机器码和符号表）
lc3_asm <input.asm> <output.bin> -d
```

**示例：**

```bash
lc3_asm test_case/asm/fibonacci.asm test_case/output/bin/fibonacci.bin
```

### 2. 模拟器 (lc3_sim)

```bash
# 基本用法
lc3_sim <bin_file>

# 完整参数
lc3_sim <bin_file> [options]
```

**可用参数：**

| Parameter | Description |
| :--- | :--- |
| `-pc <addr>` | 设置初始 PC 值 |
| `-reg <id> <val>` | 设置寄存器值（id: 0-7 为 R0-R7，8 为 PC） |
| `-mem <addr> <val>` | 设置内存值 |
| `-break <addr>` | 在指定地址设置断点 |
| `-d` | 启用交互式调试模式 |

**交互式调试命令：**

| Command | Description |
| :--- | :--- |
| `s` | 单步执行 |
| `c` | 继续运行 |
| `p` | 打印寄存器状态 |
| `m <addr>` | 查看内存地址内容 |
| `q` | 退出 |

**示例：**

```bash
# 运行程序
lc3_sim test_case/bin/fibonacci.asm.bin

# 交互式调试
lc3_sim test_case/bin/fibonacci.asm.bin -d

# 设置断点运行(未进入 debug mode，则在断点处停止)
lc3_sim test_case/bin/fibonacci.asm.bin -break x3010
```

---

## 🧪 自动化测试 (Automated Testing)

项目提供了基于 Python 的测试框架，位于 `scripts/` 目录下。

### 1. 使用“魔法注释”生成配置

> 仅服务集成测试，针对汇编器和模拟器的测试未提供此服务。

在 .asm 文件中添加注释来定义测试预期：

``` assembly
; @INIT: R0=10, PC=x3000
; @EXPECT: R0=15, R1=xABCD

.ORIG x3000
ADD R0, R0, #5
...
```

运行配置生成脚本：

```bash
# 记得在脚本处取消相应注释
python scripts/gen_config.py
```

### 2. 运行测试

使用统一入口 `scripts/run.py`：

```bash
# 运行所有测试 (汇编 + 模拟 + 集成)
python scripts/run.py

# 仅运行特定模块
python scripts/run.py --asm   # 汇编测试
python scripts/run.py --sim   # 模拟器测试
python scripts/run.py --integ # 集成测试

# 关键词过滤 (例如只测包含 "fib" 的用例)
python scripts/run.py fibonacci
```

---

## 📚 LC-3 指令集参考 (LC-3 ISA)

### 算术逻辑指令

| 指令 | 格式 | 描述 |
| :--- | :--- | :--- |
| ADD | `ADD DR, SR1, SR2` | DR = SR1 + SR2 |
| ADD | `ADD DR, SR1, imm5` | DR = SR1 + imm5 |
| AND | `AND DR, SR1, SR2` | DR = SR1 & SR2 |
| AND | `AND DR, SR1, imm5` | DR = SR1 & imm5 |
| NOT | `NOT DR, SR` | DR = ~SR |

### 数据传输指令

| 指令 | 格式 | 描述 |
| :--- | :--- | :--- |
| LD | `LD DR, LABEL` | DR = MEM[PC + offset] |
| LDI | `LDI DR, LABEL` | DR = MEM[MEM[PC + offset]] |
| LDR | `LDR DR, BaseR, offset6` | DR = MEM[BaseR + offset] |
| LEA | `LEA DR, LABEL` | DR = PC + offset |
| ST | `ST SR, LABEL` | MEM[PC + offset] = SR |
| STI | `STI SR, LABEL` | MEM[MEM[PC + offset]] = SR |
| STR | `STR SR, BaseR, offset6` | MEM[BaseR + offset] = SR |

### 控制流指令

| 指令 | 格式 | 描述 |
| :--- | :--- | :--- |
| BR | `BRnzp LABEL` | 条件分支（n=负, z=零, p=正） |
| JMP | `JMP BaseR` | 无条件跳转到 BaseR |
| RET | `RET` | 返回（JMP R7） |
| JSR | `JSR LABEL` | 子程序调用 |
| JSRR | `JSRR BaseR` | 子程序调用（寄存器） |

### 系统调用

| 指令 | 描述 |
| :--- | :--- |
| TRAP x20 (GETC) | 读取一个字符 |
| TRAP x21 (OUT) | 输出一个字符 |
| TRAP x22 (PUTS) | 输出字符串 |
| TRAP x23 (IN) | 提示并读取字符 |
| TRAP x24 (PUTSP) | 输出压缩字符串 |
| TRAP x25 (HALT) | 停机 |

---

## ⚖️ 许可证

[114August514/lc3_system] - Lab Project
本项目为原创实现，仅供学习和教学使用。
项目源码托管于本仓库，地址：[LC-3 System](https://github.com/114August514/lc3_system)。

---

## 📋 参考资料

* **教材**: Introduction to Computing Systems (Patt & Patel).
* **参考实现**: **[lc3tools](https://github.com/chiragsakhuja/lc3tools)**.
