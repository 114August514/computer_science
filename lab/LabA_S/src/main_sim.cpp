//
// Created by Admin on 26-1-10.
//

#include <stdexcept>
#include <iostream>
#include <string>

#include "lc3_vm.h"
#include "types.h"
#include "utils.h"

void ShowUsage()
{
    std::cout << "Usage:" << std::endl;
    std::cout << "  Simulate: simulator <bin_file> [options]" << std::endl;
    std::cout << "   -reg <id> <val>     Set register (id: 0-7 for R0-R7, 8 for PC)" << std::endl;
    std::cout << "   -mem <addr> <val>   Set memory value" << std::endl;
    std::cout << "   -break <addr>       Add breakpoint at address" << std::endl;
}

int main(int argc, char* argv[])
{
    // 1. 检查命令行参数
    if (argc < 2)
    {
        ShowUsage();
        return 1;
    }

    // 2. 加载机器码
    LC3_VM vm;
    string bin_path = argv[1];

    if (!vm.LoadBinFile(bin_path))
    {
        std::cout << "[Error] Failed to load binary file: " << bin_path << std::endl;
        return 1;
    }

    // 3. 解析命令行参数
    try
    {
        for (int i = 2; i < argc; i++)
        {
            string arg = argv[i];

            if (arg == "-reg")
            {
                if (i + 2 >= argc)
                    throw std::invalid_argument("Missing args for -reg");

                RegID id = static_cast<RegID>(Utils::Convert::StrToInt(argv[++i]));
                uint16_t val = static_cast<uint16_t>(Utils::Convert::StrToInt(argv[++i]));

                if (id >= REG_R0 && id <= PC)
                    vm.SetReg(id, val);
                else
                    throw std::out_of_range("RegID must be 0-7 (R0-R7) or 8 (PC).");
            }
            else if (arg == "-mem")
            {
                if (i + 2 >= argc)
                    throw std::invalid_argument("Missing args for -mem");

                uint16_t addr = static_cast<uint16_t>(Utils::Convert::StrToInt(argv[++i]));
                uint16_t val = static_cast<uint16_t>(Utils::Convert::StrToInt(argv[++i]));
                vm.SetMem(addr, val);
            }
            else if (arg == "-break")
            {
                if (i + 1 >= argc)
                    throw std::invalid_argument("Missing args for -break");

                uint16_t addr = static_cast<uint16_t>(Utils::Convert::StrToInt(argv[++i]));
                vm.AddBreakpoint(addr);
                std::cout << "[Config] Breakpoint set at x" << std::hex << addr << std::dec << std::endl;
            }
            else
                throw std::invalid_argument("Unknown args!");
        }
    }
    catch(ErrorCode code)
    {
        std::cout << "[Error] Argument parsing failed:";
        switch (code) {
            case ErrorCode::INVALID_OPERAND:   
                std::cout << "Invalid operand format."; break;
            case ErrorCode::IMM_OUT_OF_RANGE:  
                std::cout << "Immediate value out of range."; break;
            default:                          
                std::cout << "Parsing error."; break;
        }
        std::cout << std::endl;
        return 1;
    }
    catch(const std::exception& e)
    {
        std::cout << "[Error] Argument parsing failed:" << e.what() << std::endl;
        return 1;
    }
    catch(...)
    {
        std::cout << "[Error] Unknown error while parsing arguments." << std::endl;
        return 1;
    }
    
    // 4. 开始运行
    std::cout << "Simulator initialized. Ready to run." << std::endl;

    vm.Run();
    std::cout << "Simulation finished." << std::endl;
}