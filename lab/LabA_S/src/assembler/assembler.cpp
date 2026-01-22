//
// Created by Admin on 26-1-5.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include "common/utils.h"
#include "assembler/assembler.h"

// 0. 本地辅助函数
namespace
{
    OpCode GetOpType(const string& token)
    {
        static const std::unordered_map<string, OpCode> op_map = 
        {
            {"ADD",  OpCode::ADD},  {"AND",   OpCode::AND},  {"NOT",   OpCode::NOT},
            {"LD",   OpCode::LD},   {"LDR",   OpCode::LDR},  {"LDI",   OpCode::LDI},
            {"ST",   OpCode::ST},   {"STR",   OpCode::STR},  {"STI",   OpCode::STI},
            {"LEA",  OpCode::LEA},  {"JMP",   OpCode::JMP},  {"RET",   OpCode::JMP},
            {"JSR",  OpCode::JSR},  {"JSRR",  OpCode::JSRR},
            {"TRAP", OpCode::TRAP}, {"RTI",   OpCode::RTI},

            // TRAP 别名
            {"HALT", OpCode::TRAP}, {"GETC",  OpCode::TRAP}, {"OUT",   OpCode::TRAP},
            {"PUTS", OpCode::TRAP}, {"IN",    OpCode::TRAP}, {"PUTSP", OpCode::TRAP},

            // BR 别名(识别的时候已经转为大写)
            {"BR",   OpCode::BR},   {"BRN",   OpCode::BR},   {"BRZ",   OpCode::BR},
            {"BRP",  OpCode::BR},   {"BRNZ",  OpCode::BR},   {"BRNP",  OpCode::BR},
            {"BRZP", OpCode::BR},   {"BRNZP", OpCode::BR}
        };

        auto it = op_map.find(token);
        if (it != op_map.end())
            return it->second;

        return OpCode::RES;
    }

    PseudoOp GetPseudoOpType(const string& token)
    {
        static const std::unordered_map<string, PseudoOp> pseudo_map =
        {
            {".ORIG", PseudoOp::ORIG},
            {".END", PseudoOp::END},
            {".FILL", PseudoOp::FILL},
            {".BLKW", PseudoOp::BLKW},
            {".STRINGZ", PseudoOp::STRINGZ}
        };

        auto it = pseudo_map.find(token);
        if (it != pseudo_map.end())
            return it->second;

        return PseudoOp::UNKNOWN;
    }

    uint16_t GetCondCode(const string & mnemonic)
    {
        // BR = BRnzp
        if (mnemonic == "BR") return 0x7;

        uint16_t cc = 0;
        if (mnemonic.find('N') != string::npos) cc |= CondCode::NEG;
        if (mnemonic.find('Z') != string::npos) cc |= CondCode::ZER;
        if (mnemonic.find('P') != string::npos) cc |= CondCode::POS;

        return cc;
    }

    uint16_t GetTrapVector(const string& mnemonic)
    {
        static const std::unordered_map<string, uint16_t> trap_map =
        {
            {"GETC", TrapVector::TRAP_GETC},
            {"OUT", TrapVector::TRAP_OUT},
            {"PUTS", TrapVector::TRAP_PUTS},
            {"IN", TrapVector::TRAP_IN},
            {"PUTSP", TrapVector::TRAP_PUTSP},
            {"HALT", TrapVector::TRAP_HALT}
        };

        auto it = trap_map.find(mnemonic);
        if (it != trap_map.end())
            return it->second;

        return TrapVector::BAD_TRAP;
    }

    string GetErrorMessage(ErrorCode code)
    {
        static const std::unordered_map<ErrorCode, string> error_msg_map =
        {
            {ErrorCode::NONE, "No error."},
            {ErrorCode::UNKNOWN_MNEMONIC, "Unknown mnemonic or opcode."},
            {ErrorCode::INVALID_OPERAND, "Invalid operand format."},
            {ErrorCode::IMM_OUT_OF_RANGE, "Immediate value out of range."},
            {ErrorCode::LABEL_UNDEFINED, "Label undefined."},
            {ErrorCode::LABEL_REDEFINED, "Label redefined."},
            {ErrorCode::ORIG_MISSING, "Missing .ORIG directive."},
            {ErrorCode::END_MISSING, "Missing .END directive."},
            {ErrorCode::INVALID_ORIG_ADDR, "Invalid .ORIG address."},
            {ErrorCode::INVALID_STRING, "Invalid string constant."},
            {ErrorCode::ADDRESS_OVERFLOW, "Memory address overflow."}
        };

        auto it = error_msg_map.find(code);
        if (it != error_msg_map.end())
            return it->second;

        return "Unknown error."; // 虽然这条不可能运行到
    }
}

// 1. 公共接口实现
Assembler::Assembler(const string &input_path, const string &output_path)
    : input_path_(input_path), output_path_(output_path), lc_(0), has_error_(false) {}

void Assembler::PrintSymbolTable() const
{
    std::cout << "-- -Symbol Table-- -" << std::endl;

    // 用地址排序（原 map 用字典序排序）
    std::vector<std::pair<string, uint16_t>> sorted_symbols(symbol_table_.begin(), symbol_table_.end());
    std::sort(sorted_symbols.begin(), sorted_symbols.end(),
        [](const std::pair<string, uint16_t>& a, auto& b)
        {
            return a.second < b.second;
        }
    );

    for (const auto& pair : sorted_symbols)
    {
        const string& label = pair.first;
        uint16_t addr = pair.second;

        // 打印格式：
        // Label -> xAddr
        std::cout << label << " : "
                  << "x" << std::hex << std::uppercase << addr
                  << std::dec << std::endl;
    }
    std::cout << "--------------------" << std::endl;
}

bool Assembler::Run(bool debug_mode)
{
    // 第一遍扫描：构建符号表
    if (!PassOne())
    {
        for (const string& log : error_log_)
            std::cout << log << std::endl;

        return false;
    }

    if (debug_mode)
        PrintSymbolTable();

    // 第二遍扫描：生成机器码
    if (!PassTwo(debug_mode))
    {
        for (const string& log : error_log_)
            std::cout << log << std::endl;
        return false;
    }

    return true;
}

// 2. 核心函数实现
bool Assembler::PassOne()
{
    // 1. 创建并打开文件
    std::ifstream file(input_path_);
    if (!file.is_open())
    {
        HintError(0, "Cannot open Input file: " + input_path_ + ".");
        return false;
    }

    // 初始化
    source_lines_.clear();
    symbol_table_.clear();
    error_log_.clear();
    has_error_ = false;
    lc_ = 0;

    string line;
    uint16_t line_index = 0;

    // 状态标记：是否处于 .ORIG ... .END 代码块中
    bool in_block = false;

    // 2. 逐行读取
    while (std::getline(file, line))
    {
        // 2.1 缓存原始行
        source_lines_.push_back(line);
        line_index++;

        // 2.2 预处理（跳过空行）
        string cleaned = Utils::String::PreProcess(line);
        if (cleaned.empty()) continue;

        std::vector<string> tokens = Utils::String::Split(cleaned);
        if (tokens.empty()) continue;

        // 2.3 处理汇编码（主要是计算内存和写对照表）
        try
        {
            // 2.3.1 分析指令
            string label;
            int start_idx = 0; // 指令起始位置
            OpCode op = GetOpType(tokens[0]);
            PseudoOp pseudo = GetPseudoOpType(tokens[0]);

            // 判断是否有 Label（这里还没开始管是不是在代码块里）
            if (op == RES && pseudo == PseudoOp::UNKNOWN)
            {
                label = tokens[0];
                start_idx = 1;

                if (tokens.size() > 1)
                    pseudo = GetPseudoOpType(tokens[1]);
            }

            // 2.3.2 处理 .ORIG 
            if (pseudo == PseudoOp::ORIG)
            {
                // 即使缺少 .END 未构成代码块，也暂时不退出。
                if (in_block)
                    HintError(line_index, ErrorCode::END_MISSING);

                if (tokens.size() != start_idx + 2)
                    throw ErrorCode::INVALID_OPERAND;

                int32_t addr = Utils::Convert::StrToInt(tokens[start_idx + 1]);
                if (addr < 0) // addr < 0xFFFF 一定成立
                    throw ErrorCode::ADDRESS_OVERFLOW;

                lc_ = static_cast<uint16_t>(addr);
                in_block = true;

                // 处理 .ORIG 行的 Label
                if (!label.empty())
                {
                    if (symbol_table_.count(label)) throw ErrorCode::LABEL_REDEFINED;
                    symbol_table_[label] = lc_;
                }
                continue;
            }

            // 2.3.3 处理 .END
            if (pseudo == PseudoOp::END)
            {
                if (!in_block)
                    throw ErrorCode::ORIG_MISSING;

                in_block = false;
                continue;
            }

            // 2.3.4 判断代码是否包裹在 .ORIG 和 .END 内
            if (!in_block)
                throw ErrorCode::ORIG_MISSING;

            // 2.3.5 常规处理
            // 有 Label
            if (!label.empty())
            {
                if (symbol_table_.count(label)) 
                    throw ErrorCode::LABEL_REDEFINED;
                symbol_table_[label] = lc_;
            }

            uint16_t size = CalSize(tokens);
            if (static_cast<uint32_t>(lc_) + size > MEMORY_MAX)
                throw ErrorCode::ADDRESS_OVERFLOW;

            lc_ += size;
        }
        catch (ErrorCode code)
        {
            HintError(line_index, code, line);
        }
        catch (...)
        {
            HintError(line_index, "Unknown Error.");
        }
    }

    // 3. 关闭文件
    file.close();

    if (in_block)
        HintError(line_index, ErrorCode::END_MISSING);

    return !has_error_;
}

bool Assembler::PassTwo(bool debug_mode)
{
    // 为保证兼容多个 .ORIG 的输入格式，使用 std::vector<CodeBlock> 缓存机器码
    std::vector<CodeBlock> memory_buffer;

    // 1. 初始化（虽然进 Pass 2 说明 Pass 1 没有报错，但仍进行清理操作）
    has_error_ = false;
    lc_ = 0;
    bool in_block = false;
    uint16_t line_index = 0;

    // 2. 逐行处理汇编码并写入内存缓冲区（结构基本与 Pass 1 类似）
    for (const string& raw_line : source_lines_)
    {
        line_index++;
        string cleaned = Utils::String::PreProcess(raw_line);
        if (cleaned.empty()) continue;

        std::vector<string> tokens = Utils::String::Split(cleaned);
        if (tokens.empty()) continue;

        try
        {
            int start_idx = 0; // 指令起始位置
            OpCode op = GetOpType(tokens[0]);
            PseudoOp pseudo = GetPseudoOpType(tokens[0]);

            // 判断是否有 Label（这里还没开始管是不是在代码块里）
            if (op == RES && pseudo == PseudoOp::UNKNOWN)
            {
                start_idx = 1;
                if (tokens.size() > 1)
                {
                    op = GetOpType(tokens[1]);
                    pseudo = GetPseudoOpType(tokens[1]);
                }
            }

            // 处理 .ORIG
            if (pseudo == PseudoOp::ORIG)
            {
                // 参数检查 Pass 1 做过了，故默认合法
                lc_ = static_cast<uint16_t>(Utils::Convert::StrToInt(tokens[start_idx + 1]));
                in_block = true;

                CodeBlock new_block(lc_);
                memory_buffer.push_back(new_block);

                continue;
            }

            // 处理 .END
            if (pseudo == PseudoOp::END)
            {
                in_block = false;
                continue;
            }

            if (!in_block) continue; // 基本上不会运行

            // 获取当前代码块
            CodeBlock& current_block = memory_buffer.back();

            // 处理 FILL
            if (pseudo == PseudoOp::FILL)
            {
                string operand = tokens[start_idx + 1];
                uint16_t val = 0;
                try
                {
                    // a. 先尝试立即数解析
                    val = static_cast<uint16_t> (ParseImmediate(operand));
                }
                catch(...)
                {
                    // b. 如果不是立即数，则认为是 Label
                    if (symbol_table_.count(operand))
                        val = symbol_table_[operand];
                    else
                        throw ErrorCode::LABEL_UNDEFINED;
                }

                current_block.code.push_back(val);
                lc_++;
                continue;
            }

            // 处理 .BLKW
            if (pseudo == PseudoOp::BLKW)
            {
                int32_t count = Utils::Convert::StrToInt(tokens[start_idx + 1]);
                current_block.code.insert(current_block.code.end(), count, 0);
                lc_ += static_cast<uint16_t>(count);
                continue;
            }

            // 处理 .STRINGZ
            if (pseudo == PseudoOp::STRINGZ)
            {
                const string& str = tokens[start_idx + 1];
                for (size_t i = 1; i < str.size() - 1; i++)
                {
                    char c = str[i];
                    if (c == '\\')
                    {
                        if (i + 1 < str.size() - 1)
                        {
                            switch (char next_c = str[i + 1])
                            {
                                case '0':
                                    c = '\0'; break;
                                case 'n': 
                                    c = '\n'; break;
                                case 'r': 
                                    c = '\r'; break;
                                case 't': 
                                    c = '\t'; break;
                                case '\'':
                                    c = '\''; break;
                                case '"': 
                                    c = '\"'; break;
                                case '\\': 
                                    c = '\\'; break;
                                default: 
                                    c = next_c; break; // 不支持转译则直接输出下一个字符
                            }
                            i++;
                        }
                    }
                    current_block.code.push_back(static_cast<uint16_t>(c));
                    lc_++;
                }

                // 写 '\0'
                current_block.code.push_back(0);
                lc_++;
                continue;
            }

            // 标准指令
            if (op != RES)
            {
                std::vector<string> instr_tokens;
                instr_tokens.assign(tokens.begin() + start_idx, tokens.end());

                uint16_t bin = TranslateLine(instr_tokens);
                current_block.code.push_back(bin);

                if (debug_mode)
                {
                    std::cout << "x" << std::hex << std::uppercase << lc_ << std::dec << " : "
                              << Utils::Convert::U16ToBin(bin) << std::endl;
                }

                lc_++;
            }
        }
        catch(ErrorCode code)
        {
            HintError(line_index, code, raw_line);
        }
        catch(...)
        {
            HintError(line_index, "Unknown Error.");
        }
    }

    // 3. 输出文件
    if (has_error_) return false;
    return SaveBinFile(memory_buffer);
}

bool Assembler::SaveBinFile(const std::vector<CodeBlock>& blocks)
{
    // 空文件
    if (blocks.empty()) return true;

    std::ofstream file(output_path_, std::ios::binary);
    if (!file.is_open())
    {
        HintError(0, "Cannot open Output file: " + output_path_ + ".");
        return false;
    }

    // 根据 blocks 的大小决定输出格式（保证向下兼容）
    // A. 仅一个代码块（输出标准格式）
    if (blocks.size() == 1)
    {
        const CodeBlock& block = blocks[0];

        // 写入文件头（标记内存起始地址）
        Utils::IO::WriteWord(file, block.start_addr);

        // 写入数据
        for (uint16_t data : block.code)
            Utils::IO::WriteWord(file, data);
    }

    // B. 多个代码块
    else
    {
        // 写入 Magic Number 用于模拟器模式区分
        Utils::IO::WriteWord(file, LC3_NUMBER);

        // 写入程序入口
        Utils::IO::WriteWord(file, blocks[0].start_addr);

        // 写入数据
        for (const CodeBlock& block : blocks)
        {
            if (block.code.empty()) continue;

            // Header: [地址] [长度]
            Utils::IO::WriteWord(file, block.start_addr);
            Utils::IO::WriteWord(file, static_cast<uint16_t>(block.code.size()));

            // Body: [数据...]
            for (uint16_t data : block.code)
                Utils::IO::WriteWord(file, data);
        }
    }    

    file.close();
    return true;
}

// 3. 工具函数实现
void Assembler::HintError(uint16_t line_index, const string& message)
{
    std::stringstream ss;
    ss << "[Error] Line " << line_index << ": " << message;
    error_log_.push_back(ss.str());
    has_error_ = true;
}

void Assembler::HintError(uint16_t line_index, ErrorCode code, const string& detail)
{
    if (code == ErrorCode::NONE) return;
    
    string msg = GetErrorMessage(code);
    if (!detail.empty())
    {
        msg += "(" + detail + ")";
    }
    HintError(line_index, msg);
}

uint16_t Assembler::CalSize(const std::vector<string> &tokens)
{
    if (tokens.empty()) return 0;

    // 确认指令起始位置（跳过 Label)
    size_t start_idx = 0;
    OpCode op = GetOpType(tokens[0]);
    PseudoOp pseudo = GetPseudoOpType(tokens[0]);

    if (op == RES && pseudo == PseudoOp::UNKNOWN)
    {
        if (tokens.size() <= 1) return 0; // 仅有 Label

        start_idx = 1;
        op = GetOpType(tokens[1]);
        pseudo = GetPseudoOpType(tokens[1]);
    }

    // 处理标准指令
    if (op != RES)
        return 1;

    // 开始处理伪指令
    // 仅一个有效操作数
    if (tokens.size() <= start_idx + 1)
    {
        // 处理 .END
        if (pseudo == PseudoOp::END) return 0;
        if (pseudo != PseudoOp::UNKNOWN)
            throw ErrorCode::INVALID_OPERAND;

        throw ErrorCode::UNKNOWN_MNEMONIC;
    }

    // 检查操作数是否正确（2个）
    if (tokens.size() != start_idx + 2)
        throw ErrorCode::INVALID_OPERAND;

    switch (pseudo)
    {
        // 处理 .BLKW
        case PseudoOp::BLKW:
        {
            int32_t val = Utils::Convert::StrToInt(tokens[start_idx + 1]);
            if (val <= 0) // val < 0xFFFF 始终成立（原本是负数，但考虑到 0 没有意义，就也算错误了）
                throw ErrorCode::INVALID_OPERAND;

            return static_cast<uint16_t>(val);
        } 

        // 处理 .STRINGZ
        case PseudoOp::STRINGZ:
        {
            const string& str = tokens[start_idx + 1];
            if (str.size() < 2 || str.front() != '"' || str.back() != '"')
                throw ErrorCode::INVALID_STRING;

            int length = 0;
            for (size_t i = 1; i < str.size() - 1; i++)
            {
                // 跳过转义字符
                if (str[i] == '\\' && i + 1 < str.size() - 1)
                    i++;

                length++;
            }
            return length + 1;
        }

        // 处理 .FILL
        case PseudoOp::FILL:
            return 1;

        // 处理 .ORIG
        case PseudoOp::ORIG:
            return 0;
        
        default:
            throw ErrorCode::INVALID_OPERAND;
    }
}

int16_t Assembler::ParseReg(const string &token)
{
    if (token.length() != 2 || token[0] != 'R') 
        throw ErrorCode::INVALID_OPERAND;
    
    if (token[1] >= '0' && token[1] <= '7') 
        return token[1] - '0';

    throw ErrorCode::INVALID_OPERAND;
}

int16_t Assembler::ParseImmediate(const string &token)
{
    return static_cast<int16_t> (Utils::Convert::StrToInt(token));
}

uint16_t Assembler::TranslateLine(const std::vector<string>& tokens)
{
    // 伪指令不调用此函数生成机器码
    if (tokens.empty()) return 0; // 空行

    // 1. 确定指令起始位置（跳过 Label）
    size_t start_idx = 0;
    OpCode op = GetOpType(tokens[0]);
    PseudoOp pseudo = GetPseudoOpType(tokens[0]);

    if (op == RES && pseudo == PseudoOp::UNKNOWN)
    {
        if (tokens.size() <= 1) return 0; // 仅有 Label

        start_idx = 1;
        op = GetOpType(tokens[1]);
        pseudo = GetPseudoOpType(tokens[1]);
    }

    // 2. 截取指令部分的 tokens
    std::vector<string> instruction_tokens;
    for (size_t i = start_idx; i < tokens.size(); i++)
        instruction_tokens.push_back(tokens[i]);

    // 3. 跳过伪指令
    if (pseudo != PseudoOp::UNKNOWN)
        return 0;

    // 4. 处理指令
    switch (op)
    {
        case ADD:
        case AND:
        case NOT:
            return HandleOperate(instruction_tokens, op);

        case LD:
        case LDI:
        case ST:
        case STI:
        case LEA:
        case BR:
        case JSR:
            return HandlePcRelative(instruction_tokens, op);

        case LDR:
        case STR:
            return HandleBaseOffset(instruction_tokens, op);

        case JMP:
        case JSRR:
            return HandleRegOnly(instruction_tokens, op);

        case TRAP:
            return HandleTrap(instruction_tokens);

        case RTI:
            return 0x8000; // RTI 固定为 1000 0000 0000 0000

        default:
            throw ErrorCode::UNKNOWN_MNEMONIC;
    }
}

uint16_t Assembler::HandleOperate(const std::vector<string> &tokens, OpCode op)
{
    if (tokens.size() < 3)
        throw ErrorCode::INVALID_OPERAND;

    // 1. 助记符（tokens[0]）
    uint16_t machine_code = op << 12;

    // 2. 读取 DR, SR1
    int16_t dr  = ParseReg(tokens[1]);
    int16_t sr1 = ParseReg(tokens[2]);

    machine_code |= (dr << 9);
    machine_code |= (sr1 << 6);

    // 3. 处理 NOT 指令 
    // NOT DR, SR
    // 格式：1001 | DR | SR | 1 | 11111
    if (op == OpCode::NOT)
    {
        if (tokens.size() != 3)
            throw ErrorCode::INVALID_OPERAND;

        machine_code |= 0x3F;
        return machine_code;
    }

    // 4. 处理 ADD 和 AND 指令
    // ADD DR, SR1, SR2
    // ADD DR, SR, imm5
    // AND 类似
    // xxxx | DR | SR1 | 0 | 00 | SR2
    // xxxx | DR | SR  | 1 | imm5
    if (tokens.size() != 4)
        throw ErrorCode::INVALID_OPERAND;

    // 检查第三个操作数是寄存器还是立即数
    try
    {
        int16_t sr2 = ParseReg(tokens[3]);
        machine_code |= (sr2 & 0x7);
    }
    catch (ErrorCode)
    {
        // 解析寄存器失败，则尝试解析为立即数
        int16_t imm5 = ParseImmediate(tokens[3]);

        // imm5 范围 [-16, 15]
        if (imm5 < -16 || imm5 > 15)
            throw ErrorCode::IMM_OUT_OF_RANGE;

        machine_code |= (1 << 5);
        machine_code |= (imm5 & 0x1F);
    }

    return machine_code;
}

uint16_t Assembler::HandlePcRelative(const std::vector<string> &tokens, OpCode op)
{
    // PC = 当前指令地址 + 1
    uint16_t current_pc = lc_ + 1;
    uint16_t machine_code = op << 12;
    string label;

    // 1. 处理 BR
    // 0000 | n | z | p | PCoffset9
    if (op == OpCode::BR)
    {
        if (tokens.size() != 2) throw ErrorCode::INVALID_OPERAND;
        uint16_t cc = GetCondCode(tokens[0]);
        machine_code |= (cc << 9);
        label = tokens[1];
    }

    // 2. 处理 JSR
    // 0100 | 1 | PCoffset11
    else if (op == OpCode::JSR)
    {
        if (tokens.size() != 2) throw ErrorCode::INVALID_OPERAND;
        machine_code |= (1 << 11);
        label = tokens[1];
    }

    // 3. 处理三个操作数的
    // 包括 LD LDI LEA ST STI
    // xxxx | R | PCoffset9
    else
    {
        if (tokens.size() != 3)
            throw ErrorCode::INVALID_OPERAND;

        int16_t target_reg = ParseReg(tokens[1]);
        machine_code |= (target_reg << 9);
        label = tokens[2];
    }

    // 4. 计算偏移量
    auto it = symbol_table_.find(label);
    if (it == symbol_table_.end())
        throw ErrorCode::LABEL_UNDEFINED;
    
    uint16_t label_addr = it->second;
    
    // 防溢出 int32_t 
    int32_t offset = static_cast<int32_t>(label_addr) - static_cast<int32_t>(current_pc);

    // 5. 检查范围（无异常则拼接）
    // PCoffset11
    if (op == OpCode::JSR)
    {   
        if (offset < -1024 || offset > 1023) 
            throw ErrorCode::IMM_OUT_OF_RANGE;
        machine_code |= (offset & 0x7FF);
    }

    // PCoffset9
    else
    {
        if (offset < -256 || offset > 255)
            throw ErrorCode::IMM_OUT_OF_RANGE;
        machine_code |= (offset & 0x1FF);
    }
    return machine_code;
}

uint16_t Assembler::HandleBaseOffset(const std::vector<string> &tokens, OpCode op)
{
    // xxxx | R | BaseR | offset6
    if (tokens.size() != 4)
        throw ErrorCode::INVALID_OPERAND;

    uint16_t machine_code = op << 12;
    int16_t target_reg = ParseReg(tokens[1]);
    int16_t base_reg = ParseReg(tokens[2]);
    int16_t offset = ParseImmediate(tokens[3]);

    if (offset < -32 || offset > 31)
        throw ErrorCode::IMM_OUT_OF_RANGE;

    machine_code |= (target_reg << 9);
    machine_code |= (base_reg << 6);
    machine_code |= (offset & 0x3F);
    return machine_code;
}

uint16_t Assembler::HandleRegOnly(const std::vector<string> &tokens, OpCode op)
{
    // RET
    // xxxx | 000 | BaseR | 000000
    uint16_t machine_code = 0;

    // 1. 确定机器码
    if (op == OpCode::JSRR)
        machine_code = 0x4000;
    else 
        machine_code = 0xC000;
        
    // 2. 处理 RET
    if (tokens[0] == "RET")
    {
        if (tokens.size() != 1)
            throw ErrorCode::INVALID_OPERAND;
        
        machine_code |= (7 << 6);
        return machine_code;
    }

    // 3. 处理 JMP 和 JSRR
    if (tokens.size() != 2)
        throw ErrorCode::INVALID_OPERAND;
    
    int16_t base_reg = ParseReg(tokens[1]);
    machine_code |= (base_reg << 6);

    return machine_code;
}

uint16_t Assembler::HandleTrap(const std::vector<string> &tokens)
{
    // TRAP 指令格式：1111 | 0000 | trapvect8
    uint16_t machine_code = 0xF000;
    const string& mnemonic = tokens[0];

    // 1. 先判断是否为别名
    uint16_t trap_vector = GetTrapVector(mnemonic);

    // 处理指令别名
    if (trap_vector != TrapVector::BAD_TRAP)
    {
        if (tokens.size() != 1)
            throw ErrorCode::INVALID_OPERAND;

        machine_code |= (trap_vector & 0xFF);
    }

    // 2. 处理标准形式：TRAP xNN
    else if (mnemonic == "TRAP")
    {
        if (tokens.size() != 2)
            throw ErrorCode::INVALID_OPERAND;

        int16_t vec = ParseImmediate(tokens[1]);
        if (vec < 0 || vec > 0xFF)
            throw ErrorCode::IMM_OUT_OF_RANGE;

        machine_code |= (vec & 0xFF);
    }

    // 一般不会用到，纯属安全保护
    else
        throw ErrorCode::UNKNOWN_MNEMONIC;

    return machine_code;
}