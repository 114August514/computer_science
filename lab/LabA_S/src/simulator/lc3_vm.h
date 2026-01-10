//
// Created by Admin on 26-1-5.
//

#ifndef LC3_VM_H
#define LC3_VM_H

#include <set>
#include "types.h"

class LC3_VM
{
    public:
        // 构造函数：初始化 PC 和 PSR
        LC3_VM();
        ~LC3_VM() = default;

        // 加载机器码文件到内存
        // file_path: .bin 文件路径
        // return: 是否加载成功
        bool LoadBinFile(const string& file_path);

        // 初始化相关
        void SetReg(RegID id, uint16_t val);
        void SetMem(uint16_t addr, uint16_t val);

        // 执行控制
        bool Step();    // 单步执行（辅助调试）
        void Run();     // 连续运行，直到遇到 HALT，断点，异常

        // 断点设置
        void AddBreakpoint(uint16_t addr);
        void RemoveBreakpoint(uint16_t addr);

        // 状态查看
        void PrintReg() const;
        void PrintMem(uint16_t start_addr, uint16_t end_addr) const;
        uint32_t GetInstrCount() const;

    private:
        // 1. 成员变量
        // a. 硬件状态
        uint16_t memory_[MEMORY_MAX]{};           // 内存
        uint16_t reg_[REG_COUNT]{};               // 寄存器

        // b. 运行控制
        uint32_t instr_count_;                    // 当前已执行指令数
        std::set<uint16_t> breakpoints_;          // 断点位置
        bool running_;                            // 是否运行
        bool is_halted_;                          // 是否终止

        // 2. 核心函数
        // a. 取指：IR = MEM[PC], PC++;
        void Fetch();

        // b. 译码
        OpCode Decode();

        // c. 执行单条指令
        void Execute(OpCode op);

        // d. 更新条件码
        void UpdateCC(uint16_t reg);

        // 3. 辅助函数
        // 检查是否非法访问
        bool CheckAccess(uint16_t addr);

        // 内存读取
        uint16_t MemRead(uint16_t addr);

        // 内存写入
        void MemWrite(uint16_t addr, uint16_t val);

        // 异常处理
        void RaiseException(Exception e);

        // 指令集实现
        // 算术逻辑
        void OpADD();
        void OpAND();
        void OpNOT();

        // 数据传输
        void OpLD();
        void OpLDI();
        void OpLDR();
        void OpLEA();
        void OpST();
        void OpSTI();
        void OpSTR();

        // 程序调度
        void OpJMP(); // 含 RET
        void OpJSR(); // 含 JSRR
        void OpBR();

        // 系统调用
        void OpTRAP();
        void OpRTI();

        // TRAP 历程实现
        void TrapPUTS();
        void TrapGETC();
        void TrapOUT();
        void TrapIN();
        void TrapPUTSP();
        void TrapHALT();
};


#endif //LC3_VM_H
