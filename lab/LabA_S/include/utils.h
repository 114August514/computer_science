//
// Created by Admin on 26-1-5.
//

// 该文件用于存放通用的工具函数
// Header-Only 模式

#ifndef UTILS_H
#define UTILS_H

#include <stdexcept>
#include <vector>
#include "types.h"

namespace Utils
{
    // 1. 位运算类（模拟器使用）
    namespace Bit
    {
        // 将 n-bit 的立即数扩展为 16-bit.
        // 输入：立即数 x（默认高位为 0）, 位数 bit_count;
        // 输出：16-bit 字符.
        inline uint16_t SignExtend(uint16_t x, int bit_count)
        {
            if (bit_count >= 16)
                return x;

            if (x >> (bit_count - 1) & 1)
                x |= (0xFFFF << bit_count);
            return x;
        }

        // 从 val 中提取 [end, start] 之间的位段.
        // 输入：源数据 val, 高位索引 end, 低位索引 start;
        // 输出：提取出的值（右对齐）.
        inline uint16_t GetBits(uint16_t val, int end, int start)
        {
            uint16_t mask = (1 << (end - start + 1)) - 1;
            return (val >> start) & mask;
        }
    }
    
    // 2. 字符串处理类（汇编器使用）
    namespace String
    {
        // 清理字符串首尾空白字符.
        // 输入：源字符串（不允许修改）;
        // 输出：去除首尾空白字符的新字符串.
        inline string Trim(const string& s)
        {
            size_t start = 0;
            while (start < s.length() && s[start] <= 32)
                start++;

            if (start == s.length()) return "";
            
            size_t end = s.length() - 1;
            while (end > start && s[end] <= 32)
                end--;

            // substr(起始位置, 长度)
            // substr(起始位置) 默认到末尾
            return s.substr(start, end - start + 1);
        }

        // 预处理指令行.
        // 输入：源字符串（副本）;
        // 输出：标准字符串.
        inline string PreProcess(string s)
        {
            size_t len = 0;
            bool in_string = false;

            // 1. 去除注释，替换逗号，转换大小写（防止""中的内容被处理）
            for (char& c : s)
            {
                if (c == '"')
                {
                    in_string = !in_string;

                }

                if (!in_string)
                {
                    if (c == ';') break;
                    if (c == ',') c = ' ';
                    if (c >= 'a' && c <= 'z')
                        c -= 32;
                }
                len++;
            }
            s = s.substr(0, len);

            // 2. 去除首尾空白
            return Trim(s);
        }

        // 拆分指令行（需要保护""中内容）.
        // 输入：标准字符串;
        // 输出：拆分字符串数组.
        inline std::vector<string> Split(const string& cleaned)
        {
            std::vector<string> tokens;
            string current_token;
            bool in_string = false;

            for (char c : cleaned)
            {
                if (c == '"')
                {
                    in_string = !in_string;
                    current_token += c;
                    continue;
                }

                if (in_string)
                {
                    current_token += c;
                    continue;
                }

                // 遇到空白字符
                if (c <= 32)
                {
                    if (!current_token.empty())
                    {
                        tokens.push_back(current_token);
                        current_token.clear();
                    }
                }
                else current_token += c;
            }

            if (!current_token.empty())
                tokens.push_back(current_token);
            
            return tokens;
        }
    }

    // 3. 数据类型转换类
    namespace Convert
    {
        // 将数值转换成 Bin 字符串.
        // 输入：十六进制数;
        // 输出：二进制字符串.
        inline string U16ToBin(uint16_t val)
        {
            string bin(16, '0');
            for (int i = 0; i < 16; i++)
                if ((val >> (15 - i)) & 1)
                    bin[i] = '1';

            return bin;
        }

        // 将二进制字符串转换成数值.
        // 输入：二进制字符串;
        // 输出：16-bit 字符.
        inline uint16_t BinToU16(const string& bin)
        {
            uint16_t val = 0;
            for (char c : bin)
                val = (val << 1) + c - '0';

            return val;
        }

        // 解析字符串为整数.
        // 支持格式：x0A (十六进制), #10 (十进制), 10 (无前缀十进制).
        // 输入：可被解析的字符串（默认可解析）;
        // 输出：整数.
        inline int32_t StrToInt(const string& token)
        {
            if (token.empty())
                throw ErrorCode::INVALID_OPERAND;

            size_t idx = 0;
            size_t offset = 1;
            int32_t val = 0;

            // std::stoi(str, pos, base):
            // str: 要转换的字符串;
            // pos: 传递第一个未被转换的字符下标(要求指针变量);
            // base: 进制.
            try
            {
                if (token.size() > 1)
                {
                    if (token[0] == 'X' || token[0] == 'x')
                        val = std::stoi(token.substr(1), &idx, 16);
                    else if (token[0] == '#')
                        val = std::stoi(token.substr(1), &idx, 10);
                    else
                    {
                        val = std::stoi(token, &idx, 10);
                        offset = 0;
                    }
                }
                else
                {
                    val = std::stoi(token, &idx, 10);
                    offset = 0;
                }
            }

            // 仅用以捕获 stoi 失败的异常
            catch (const std::out_of_range&)
            {
                throw ErrorCode::IMM_OUT_OF_RANGE;
            }
            catch (...)
            {
                throw ErrorCode::INVALID_OPERAND;
            }

            if (idx + offset != token.size())
                throw ErrorCode::INVALID_OPERAND;

            if (val < -32768 || val > 65535)
                throw ErrorCode::IMM_OUT_OF_RANGE;

            return val;
        }
    }

    // 4. 输入输出工具
    namespace IO
    {
        // 采用大端序写入 16 位字到输出流
        inline void WriteWord(std::ostream& os, uint16_t val)
        {
            uint8_t buffer[2];
            buffer[0] = (val >> 8) & 0xFF; // 高 8 位
            buffer[1] = val & 0xFF;        // 低 8 位
            os.write(reinterpret_cast<const char *>(buffer), 2);
        }

        // 从输入流读取大端序的 16 位字
        // 读取成功返回 true，失败返回 false（未读到完整字符等（包括 EOF））
        inline bool ReadWord(std::istream& is, uint16_t& val)
        {
            char buffer[2];
            if (!is.read(buffer, 2)) return false;

            val = (static_cast<uint8_t>(buffer[0]) << 8) | static_cast<uint8_t>(buffer[1]);
            return true;
        }
    } 
    
}
#endif //UTILS_H
