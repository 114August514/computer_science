//
// Created by Admin on 26-1-5.
//

#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include "types.h"
#include "utils.h"
#include "lc3_vm.h"

// 0. 本地辅助函数
namespace
{
    string GetExceptionMessage(Exception e)
    {
        static const std::unordered_map<Exception, string> excep_msg_map =
            {
                {Exception::ACV, "Access Control Violation."},
                {Exception::ILLEGAL_OPCODE, "Illegal Opcode."},
                {Exception::PMV, "Privilege Mode Violation."},
                {Exception::TIMEOUT, "Execution Timeout."}
            };

        auto it = excep_msg_map.find(e);
        if (it != excep_msg_map.end())
            return it->second;

        return "Unknown Exception."; // 虽然这条不可能运行到
    }
}

// 1. 公共接口实现 
LC3_VM::LC3_VM()
{
    // 设置 PC 的默认地址
    reg_[PC] = PC_START;

    // 设置 PSR
    // 默认为：用户模式，CC = Z
    reg_[PSR] = 0x8002;

    running_ = false;
    is_halted_ = false;

    instr_count_ = 0;
}

bool LC3_VM::LoadBinFile(const string& file_path)
{
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "[Error] Cannot open file: " << file_path << std::endl;
        return false;
    }

    // 1. 读取起始地址
    uint16_t origin = 0;
    if (!Utils::IO::ReadWord(file, origin))
    {
        std::cout << "[Error] Cannot read file correctly!" << std::endl;
        return false;
    }

    // 2. 加载指令到内存
    reg_[PC] = origin;
    uint32_t current_addr = origin;
    uint16_t instr = 0;

    while (Utils::IO::ReadWord(file, instr))
    {
        if (current_addr >= MEMORY_MAX)
        {
            std::cout << "[Error] Memory address overflow!" << std::endl;
            break;
        }
        memory_[current_addr++] = instr;
    }

    file.close();
    return true;
}

void LC3_VM::SetReg(RegID id, uint16_t val)
{
    reg_[id] = val;
}

void LC3_VM::SetMem(uint16_t addr, uint16_t val)
{
    memory_[addr] = val;
}

bool LC3_VM::Step()
{
    // running_ = false 表示暂停或关机
    if (!running_) return false;

    try
    {
        Fetch();
        OpCode op = Decode();
        Execute(op);

        return true;
    }
    catch(Exception e)
    {
        RaiseException(e);
        return false;
    }
}

void LC3_VM::Run()
{
    // 0. 启动检查
    running_ = true;

    // 判断之前是否彻底跑完停机
    if (is_halted_) 
    {
        instr_count_ = 0;
        memory_[MMIO::MCR] = 0x8000; // MCR 初始状态：Bit 15 置 1（Enable），重启 MCR
        is_halted_ = false;
    }

    // 如果当前是断点，强制往后走一步
    if (breakpoints_.count(reg_[PC]))
        if (Step())
            instr_count_++;

    try
    {
        while (running_)
        {
            // 1. 断点检查
            if (breakpoints_.count(reg_[PC]))
            {
                std::cout << "[DEBUG] Breakpoint hit at x"
                          << std::hex << reg_[PC] << std::dec << std::endl;

                PrintReg();
                running_ = false; // 遇到断点暂停
                return;
            }

            // 2. 超时检查
            if (instr_count_ >= MAX_INSTR_LIMIT)
                throw Exception::TIMEOUT;

            // 3. 执行指令
            if(Step())
                instr_count_++;
        }
    }
    catch (Exception e)
    {
        // 这里仅会捕获 Exception::TIMEOUT
        RaiseException(e);
    }

    // 正常运行结束
    if (is_halted_)
    {
        PrintReg();
        std::cout << "Instr Count: " << GetInstrCount() << std::endl;
    }
}

void LC3_VM::AddBreakpoint(uint16_t addr)
{
    breakpoints_.insert(addr);
}

void LC3_VM::RemoveBreakpoint(uint16_t addr)
{
    breakpoints_.erase(addr);
}

void LC3_VM::PrintReg() const
{
    // 学的示例的符号表风格
    std::cout << "-- -Register Table-- -" << std::endl;
    for (uint16_t i = 0; i <= REG_R7; i++)
    {
        std::cout << "R" << i << ": x" << std::hex << reg_[i] 
                  << std::dec << " (#" << static_cast<int16_t>(reg_[i]) << ")"
                  << std::endl;
    }

    std::cout << "PC: x"
              << std::hex << reg_[PC] << std::dec << std::endl;

    std::cout << "IR: x"
              << std::hex << reg_[IR] << std::dec << std::endl;
}

void LC3_VM::PrintMem(uint16_t start_addr, uint16_t end_addr) const
{
    std::cout << "-- -Memory Table[x" 
              << std::hex << start_addr << " - x" 
              << end_addr << "]- --" << std::endl;

    for (uint32_t i = start_addr; i <= end_addr && i < MEMORY_MAX; i++)
    {
        std::cout << "x" << i << ": x" << memory_[i] << std::endl;
    }

    std::cout << std::dec;
}

uint32_t LC3_VM::GetInstrCount() const
{
    return instr_count_;
}

void LC3_VM::Fetch()
{
    // IR = MEM[PC]
    reg_[IR] = MemRead(reg_[PC]);
    
    // PC 自增
    reg_[PC]++;
}

OpCode LC3_VM::Decode()
{
    uint16_t op = Utils::Bit::GetBits(reg_[IR], 15, 12);
    return static_cast<OpCode>(op);
}

void LC3_VM::Execute(OpCode op)
{
    switch(op)
    {
        // 算术逻辑
        case ADD: OpADD(); break;
        case AND: OpAND(); break;
        case NOT: OpNOT(); break;

        // 数据传输
        case LD:  OpLD();  break;
        case LDI: OpLDI(); break;
        case LDR: OpLDR(); break;
        case LEA: OpLEA(); break;
        case ST:  OpST();  break;
        case STI: OpSTI(); break;
        case STR: OpSTR(); break;

        // 程序调度
        case BR:  OpBR();  break;
        case JMP: OpJMP(); break;
        case JSR: OpJSR(); break;
        
        // 系统调用
        case TRAP: OpTRAP(); break;
        case RTI: OpRTI(); break;

        // 错误检查
        default:
            throw Exception::ILLEGAL_OPCODE;
    }
}

void LC3_VM::UpdateCC(uint16_t reg)
{
    uint16_t val = reg_[reg];

    // 消除当前 CC(PSR 后三位)
    reg_[PSR] &= 0xFFF8;

    // 负数
    if ((val >> 15) == 1)
        reg_[PSR] |= CondCode::NEG;

    else if (val == 0)
        reg_[PSR] |= CondCode::ZER;

    else 
        reg_[PSR] |= CondCode::POS;
}

bool LC3_VM::CheckAccess(uint16_t addr)
{
    // 用户模式：
    // 不允许访问 0x0000-0x2FFF；
    // 可自由读写 0x3000-0xFDFF；
    // 不允许访问 0xFE00-0xFFFF.
    bool is_user_mode = ((reg_[PSR] & 0x8000) != 0);
    if (is_user_mode)
    {
        if (addr < 0x3000 || addr > 0xFDFF)
            throw Exception::ACV;
    }
    return true;
}

uint16_t LC3_VM::MemRead(uint16_t addr)
{
    CheckAccess(addr);

    // 硬件行为模拟（MMIO）
    {
        // 暂时先空着，等之后再实现
        // 留个 TODO。
    }

    // 普通内存读取
    return memory_[addr];
}

void LC3_VM::MemWrite(uint16_t addr, uint16_t val)
{
    CheckAccess(addr);

    // 特权模式的先不写
    {
        // TODO
    }

    memory_[addr] = val;
}

void LC3_VM::RaiseException(Exception e)
{
    std::cout << "[Exception] " << GetExceptionMessage(e) << std::endl;
    running_ = false;
}

void LC3_VM::OpADD()
{
    // 1. 获取操作数
    // DR (Bits 11-9)
    uint16_t dr = Utils::Bit::GetBits(reg_[IR], 11, 9);

    // SR1 (Bits 8-6)
    uint16_t sr1 = Utils::Bit::GetBits(reg_[IR], 8, 6);

    // Mode (Bits 5)
    uint16_t mode = Utils::Bit::GetBits(reg_[IR], 5, 5);

    // 2. 计算
    uint16_t val1 = reg_[sr1];
    uint16_t val2 = 0;

    // 根据 mode 计算
    if (mode == 1)
    {
        uint16_t imm5 = Utils::Bit::GetBits(reg_[IR], 4, 0);
        val2 = Utils::Bit::SignExtend(imm5, 5);
    }
    else
    {
        uint16_t sr2 = Utils::Bit::GetBits(reg_[IR], 2, 0);
        val2 = reg_[sr2];
    }

    reg_[dr] = val1 + val2;
    UpdateCC(dr);
}

void LC3_VM::OpAND()
{
    // 1. 获取操作数
    // DR (Bits 11-9)
    uint16_t dr = Utils::Bit::GetBits(reg_[IR], 11, 9);

    // SR1 (Bits 8-6)
    uint16_t sr1 = Utils::Bit::GetBits(reg_[IR], 8, 6);

    // Mode (Bits 5)
    uint16_t mode = Utils::Bit::GetBits(reg_[IR], 5, 5);

    // 2. 计算
    uint16_t val1 = reg_[sr1];
    uint16_t val2 = 0;

    // 根据 mode 计算
    if (mode == 1)
    {
        uint16_t imm5 = Utils::Bit::GetBits(reg_[IR], 4, 0);
        val2 = Utils::Bit::SignExtend(imm5, 5);   
    }
    else
    {
        uint16_t sr2 = Utils::Bit::GetBits(reg_[IR], 2, 0);
        val2 = reg_[sr2];
    }

    reg_[dr] = val1 & val2; // 复制粘贴上面的，仅改了这一个东西
    UpdateCC(dr);
}

void LC3_VM::OpNOT()
{
    // DR (Bits 11-9)
    uint16_t dr = Utils::Bit::GetBits(reg_[IR], 11, 9);

    // SR1 (Bits 8-6)
    uint16_t sr = Utils::Bit::GetBits(reg_[IR], 8, 6);

    reg_[dr] = ~reg_[sr];
    UpdateCC(dr);
}

void LC3_VM::OpLD()
{
    // 1. 获取操作数
    // DR (Bits 11-9)
    uint16_t dr = Utils::Bit::GetBits(reg_[IR], 11, 9);

    // PCoffset9 (Bits 8-0)
    uint16_t pc_offset9 = Utils::Bit::GetBits(reg_[IR], 8, 0);
    uint16_t offset = Utils::Bit::SignExtend(pc_offset9, 9);

    // 2. 计算地址，并进行内存读写操作
    uint16_t addr = reg_[PC] + offset;
    reg_[dr] = MemRead(addr);

    // 3. 更新条件码
    UpdateCC(dr);
}

void LC3_VM::OpLDI()
{
    // 1. 获取操作数
    // DR (Bits 11-9)
    uint16_t dr = Utils::Bit::GetBits(reg_[IR], 11, 9);

    // PCoffset9 (Bits 8-0)
    uint16_t pc_offset9 = Utils::Bit::GetBits(reg_[IR], 8, 0);
    uint16_t offset = Utils::Bit::SignExtend(pc_offset9, 9);

    // 2. 计算地址，并进行内存读写操作
    uint16_t addr = reg_[PC] + offset;
    addr = MemRead(addr);
    reg_[dr] = MemRead(addr);

    // 3. 更新条件码
    UpdateCC(dr);
}

void LC3_VM::OpLDR()
{
    // 1. 获取操作数
    // DR (Bits 11-9)
    uint16_t dr = Utils::Bit::GetBits(reg_[IR], 11, 9);

    // BaseR (Bits 8-6)
    uint16_t base_r = Utils::Bit::GetBits(reg_[IR], 8, 6);

    // Offset6 (Bits 5-0)
    uint16_t offset6 = Utils::Bit::GetBits(reg_[IR], 5, 0);
    uint16_t offset = Utils::Bit::SignExtend(offset6, 6);

    // 2. 计算地址，并进行内存读写操作
    uint16_t addr = reg_[base_r] + offset;
    reg_[dr] = MemRead(addr);

    // 3. 更新条件码
    UpdateCC(dr);
}

void LC3_VM::OpLEA()
{
    // 1. 获取操作数
    // DR (Bits 11-9)
    uint16_t dr = Utils::Bit::GetBits(reg_[IR], 11, 9);

    // PCoffset9 (Bits 8-0)
    uint16_t pc_offset9 = Utils::Bit::GetBits(reg_[IR], 8, 0);
    uint16_t offset = Utils::Bit::SignExtend(pc_offset9, 9);

    // 2. 计算地址
    reg_[dr] = reg_[PC] + offset;
}

void LC3_VM::OpST()
{
    // 1. 获取操作数
    // SR (Bits 11-9)
    uint16_t sr = Utils::Bit::GetBits(reg_[IR], 11, 9);

    // PCoffset9 (Bits 8-0)
    uint16_t pc_offset9 = Utils::Bit::GetBits(reg_[IR], 8, 0);
    uint16_t offset = Utils::Bit::SignExtend(pc_offset9, 9);

    // 2. 计算地址，并进行内存读写操作
    uint16_t addr = reg_[PC] + offset;
    MemWrite(addr, reg_[sr]);
}

void LC3_VM::OpSTI()
{
    // 1. 获取操作数
    // SR (Bits 11-9)
    uint16_t sr = Utils::Bit::GetBits(reg_[IR], 11, 9);

    // PCoffset9 (Bits 8-0)
    uint16_t pc_offset9 = Utils::Bit::GetBits(reg_[IR], 8, 0);
    uint16_t offset = Utils::Bit::SignExtend(pc_offset9, 9);

    // 2. 计算地址，并进行内存读写操作
    uint16_t addr = reg_[PC] + offset;
    addr = MemRead(addr);
    MemWrite(addr, reg_[sr]);
}

void LC3_VM::OpSTR()
{
    // 1. 获取操作数
    // SR (Bits 11-9)
    uint16_t sr = Utils::Bit::GetBits(reg_[IR], 11, 9);

    // BaseR (Bits 8-6)
    uint16_t base_r = Utils::Bit::GetBits(reg_[IR], 8, 6);

    // Offset6 (Bits 5-0)
    uint16_t offset6 = Utils::Bit::GetBits(reg_[IR], 5, 0);
    uint16_t offset = Utils::Bit::SignExtend(offset6, 6);

    // 2. 计算地址，并进行内存读写操作
    uint16_t addr = reg_[base_r] + offset;
    MemWrite(addr, reg_[sr]);
}

void LC3_VM::OpJMP()
{
    // BaseR (Bits 8-6)
    uint16_t base_r = Utils::Bit::GetBits(reg_[IR], 8, 6);
    
    uint16_t addr = reg_[base_r];
    CheckAccess(addr);
    reg_[PC] = addr;
}

void LC3_VM::OpJSR()
{
    uint16_t addr;

    // Mode (Bits 11)
    uint16_t mode = Utils::Bit::GetBits(reg_[IR], 11, 11);
    reg_[REG_R7] = reg_[PC];

    if (mode == 0) // JSRR
    {
        // BaseR (Bits 8-6)
        uint16_t base_r = Utils::Bit::GetBits(reg_[IR], 8, 6);
        addr = reg_[base_r];
    }
    else
    {
        // PCoffset11 (Bits 10-0)
        uint16_t pc_offset11 = Utils::Bit::GetBits(reg_[IR], 10, 0);
        uint16_t offset = Utils::Bit::SignExtend(pc_offset11, 11);
        addr = reg_[PC] + offset;
    }

    CheckAccess(addr);
    reg_[PC] = addr;
}

void LC3_VM::OpBR()
{
    // 条件标志 (Bits 11-9)
    uint16_t nzp = Utils::Bit::GetBits(reg_[IR], 11, 9);

    // 状态码
    uint16_t cc = Utils::Bit::GetBits(reg_[PSR], 2, 0);
    bool ben = ((nzp & cc) != 0);
  
    if (!ben) return;

    // PCoffset9 (Bits 8-0)
    uint16_t pc_offset9 = Utils::Bit::GetBits(reg_[IR], 8, 0);
    uint16_t offset = Utils::Bit::SignExtend(pc_offset9, 9);
    uint16_t addr = reg_[PC] + offset;

    CheckAccess(addr);
    reg_[PC] = addr;
}

void LC3_VM::OpTRAP()
{
    // 0. 暂存当前状态
    uint16_t temp_psr = reg_[PSR];
    uint16_t temp_pc = reg_[PC];

    // 1. 特权级检查与栈转换
    // a. 检查 PSR
    if ((reg_[PSR] & 0x8000) != 0)
    {
        reg_[USP] = reg_[REG_R6];   // 保存用户指针
        reg_[REG_R6] = reg_[SSP];   // 加载系统栈
    }

    // b. 保护现场，c. 进入特权模式（实际上应该先保护现场的，但是因为我没写强制读写，所以小切换了下顺序）
    reg_[PSR] &= 0x7FFF;
    MemWrite(--reg_[REG_R6], temp_psr);
    MemWrite(--reg_[REG_R6], temp_pc);

    // 2. 加载服务历程
    // TRAP Vector (Bits 7-0)
    uint16_t trap_vector = Utils::Bit::GetBits(reg_[IR], 7, 0);
    
    // 2.5 绿色通道
    switch (trap_vector)
    {
        case TRAP_HALT: TrapHALT(); return;
        
        // 未实现的 TRAP
        case TRAP_GETC:
        case TRAP_OUT:
        case TRAP_PUTS:
        case TRAP_IN:
        case TRAP_PUTSP: TrapHALT(); return;
    }

    reg_[PC] = MemRead(trap_vector); // 然后执行，之后用 RTI 返回
}

void LC3_VM::OpRTI()
{
    if ((reg_[PSR] & 0x8000) != 0)
        throw Exception::PMV;

    // 1. 弹出 PC 和 PSR
    reg_[PC] = MemRead(reg_[REG_R6]++);
    reg_[PSR] = MemRead(reg_[REG_R6]++);

    // 2. 检查是否需要切换堆栈
    if ((reg_[PSR] & 0x8000) != 0)
    {
        reg_[SSP] = reg_[REG_R6];   // 保存系统指针
        reg_[REG_R6] = reg_[USP];   // 加载用户栈
    }
}

void LC3_VM::TrapHALT()
{
    std::cout << "Halting the machine..." << std::endl;

    // 清除 MCR 的 Bit 15
    uint16_t mcr_val = MemRead(MMIO::MCR);
    MemWrite(MMIO::MCR, mcr_val & 0x7FFF);

    // 软件状态更新
    running_ = false;
    is_halted_ = true;
}