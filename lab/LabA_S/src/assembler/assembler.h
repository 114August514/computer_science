//
// Created by Admin on 26-1-5.
//

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <map>
#include <vector>
#include <fstream>
#include "types.h"

class Assembler
{
    public:
        // 构造函数.
        // input_path: 汇编源代码路径 (.asm)
        // output_path: 机器码输出路径 (.bin / .obj)
        Assembler(const string& input_path, const string& output_path);
        ~Assembler() = default;

        // 输出符号表 （Label -> Address）
        void PrintSymbolTable() const;

        // 启动汇编流程.
        // debug_mode：是否开启调试模式（输出每条指令的地址和机器码）.
        // return：true 表示汇编成功，false 表示失败 
        bool Run(bool debug_mode = false);

    private:
        // 0. 定义代码段结构
        struct CodeBlock
        {
            uint16_t start_addr;                    // 当前段的起始地址
            std::vector<uint16_t> code;             // 连续的机器码

            CodeBlock() : start_addr(0) {}
            CodeBlock(uint16_t addr) : start_addr(addr) {}
        };

        // 1. 成员变量
        string input_path_;
        string output_path_;
        uint16_t lc_;                               // 当前地址计数器
        std::map<string, uint16_t> symbol_table_;   // 符号表
        std::vector<string> source_lines_;          // 源文件缓存
        std::vector<string> error_log_;             // 错误日志列表
        bool has_error_;                            // 是否有错误

        // 2. 核心流程
        // 第一遍扫描：计算地址，建立符号表
        bool PassOne();
        
        // 第二遍扫描：翻译指令，生成机器码 
        bool PassTwo(bool debug_mode);

        // 将处理完的代码块输出到二进制文件中
        bool SaveBinFile(const std::vector<CodeBlock>& blocks);

        // 3. 工具函数
        // 格式化并记录汇编过程中发生的错误.
        // line_index：行号（从第一行开始）（文件没打开则是第 0 行）;
        // message：错误描述.
        void HintError(uint16_t line_index, const string& message);
        void HintError(uint16_t line_index, ErrorCode code, const string& detail = "");

        // 计算单行指令占用内存大小.
        // tokens：切分后的指令单词;
        // return：占用字数.
        static uint16_t CalSize(const std::vector<string>& tokens);

        // 解析寄存器
        static int16_t ParseReg(const string& token);

        // 解析操作数
        static int16_t ParseImmediate(const string& token);

        // 将单行指令翻译为机器码
        // tokens：切分后的指令单词;
        // current_address：当前指令地址；
        // return：16位机器码
        uint16_t TranslateLine(const std::vector<string>& tokens, uint16_t current_address);

        // 处理 ADD AND NOT
        static uint16_t HandleOperate(const std::vector<string> &tokens, OpCode op);
        
        // 处理 LD LDI ST STI LEA BR JSR
        uint16_t HandlePcRelative(const std::vector<string> &tokens, OpCode op);
        
        // 处理 LDR STR
        static uint16_t HandleBaseOffset(const std::vector<string> &tokens, OpCode op);
        
        // 处理 JMP JSRR
        static uint16_t HandleRegOnly(const std::vector<string> &tokens, OpCode op);
        
        // 处理 TRAP
        static uint16_t HandleTrap(const std::vector<string> &tokens);
};



#endif //ASSEMBLER_H
