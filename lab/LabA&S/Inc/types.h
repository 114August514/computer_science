//
// Created by Admin on 26-1-5.
//

// 该文件用于存放通用的数据类型定义
/* 在本文件中说明项目的命名规范：
   1. 类型名（类、结构体、命名空间、函数名等）使用 PascalCase；
   2. 常量 和 枚举成员 使用 ALL_CAPS；
   3. 变量采用 snake_case；
   4. 部分命名空间采用缩写，故 ALL_CAPS。
*/

#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <cstddef>
#include <string>

// 1. 常用数据类型别名
using uint32_t = std::uint32_t;
using uint16_t = std::uint16_t;
using uint8_t = std::uint8_t;
using int32_t = std::int32_t;
using int16_t = std::int16_t;
using string = std::string;

// 2. 常量表
const size_t MEMORY_MAX = 65536;
const uint16_t PC_START = 0x3000;

// 3. 指令操作码
enum OpCode : uint16_t
{
    BR = 0,
    ADD,
    LD,
    ST,
    JSR,
    AND,
    LDR,
    STR,
    RTI,
    NOT,
    LDI,
    STI,
    JMP,
    RES,                // 保留（Reserved），也充当错误码
    LEA,
    TRAP,
    JSRR                // 仅用于汇编器内部标识
};

// 4. 伪指令类型
enum class PseudoOp
{
    ORIG = 0,
    FILL,
    BLKW,
    STRINGZ,
    END,
    UNKNOWN
};

// 5. 条件码
enum CondCode : uint16_t
{
    NEG = 1 << 2,       // N
    ZER = 1 << 1,       // Z
    POS = 1 << 0        // P
};

// 6. 错误码（汇编检查）
enum class ErrorCode
{
    NONE = 0,
    UNKNOWN_MNEMONIC,   // 未知助记符（包括操作码和寄存器编号）
    INVALID_OPERAND,    // 非法操作数（格式错误）
    IMM_OUT_OF_RANGE,   // 立即数超出允许范围
    LABEL_UNDEFINED,    // 标签未定义
    LABEL_REDEFINED,    // 标签重复定义
    ORIG_MISSING,       // 缺少 .ORIG 伪指令
    END_MISSING,        // 缺少 .END 伪指令
    INVALID_ORIG_ADDR,  // .ORIG 起始地址非法
    INVALID_STRING,     // .STRINGZ 字符串格式错误（主要是缺少引号）
    ADDRESS_OVERFLOW    // 地址越界（超过 xFFFF）
};

// 7. 异常（运行检查）
enum class Exception
{
    ACV = 0,            // 访问控制违规
    ILLEGAL_OPCODE,     // 非法操作码（主要是越权使用 RTI）
    PMV                 // 特权模式违规
};

// 8. 存储相关
enum RegID : int16_t    // 寄存器标识符
{
    // 通用寄存器
    // REG_R5：通常作为 FP；
    // REG_R6：通常作为 SP；
    // REG_R7：通常作为 RA；
    REG_R0 = 0, REG_R1, REG_R2, REG_R3, REG_R4, REG_R5, REG_R6, REG_R7,

    // 专用寄存器
    PC, 
    PSR,
    IR,

    // 系统寄存器
    USP,
    SSP,

    // 计数器
    REG_COUNT
};

namespace MMIO          // 内存映射 I/O 地址
{
    const uint16_t KBSR = 0xFE00;   // 键盘状态寄存器
    const uint16_t KBDR = 0xFE02;   // 键盘数据寄存器
    const uint16_t DSR  = 0xFE04;   // 显示器状态寄存器
    const uint16_t DDR  = 0xFE06;   // 显示器数据寄存器
    const uint16_t MCR  = 0xFFFE;   // 机器控制寄存器
}

enum TrapVector : uint16_t      // 陷阱向量表
{
    TRAP_GETC  = 0x20,
    TRAP_OUT   = 0x21,
    TRAP_PUTS  = 0x22,
    TRAP_IN    = 0x23,
    TRAP_PUTSP = 0x24,
    TRAP_HALT  = 0x25,
    BAD_TRAP   = 0x100,
};

#endif //TYPES_H

