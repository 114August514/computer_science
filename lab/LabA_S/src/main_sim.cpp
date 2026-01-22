//
// Created by Admin on 26-1-10.
//

#include <stdexcept>
#include <iostream>
#include <limits>
#include <string>

#include "simulator/lc3_vm.h"
#include "common/types.h"
#include "common/utils.h"

void ShowUsage()
{
    std::cout << "LC-3 Simulator" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  Simulate: lc3_sim <bin_file> [options]" << std::endl;
    std::cout << "   -pc <addr>          Set the initial PC" << std::endl;
    std::cout << "   -reg <id> <val>     Set register (id: 0-7 for R0-R7, 8 for PC)" << std::endl;
    std::cout << "   -mem <addr> <val>   Set memory value" << std::endl;
    std::cout << "   -break <addr>       Add breakpoint at address" << std::endl;
    std::cout << "   -d                  Enable interactive debug mode" << std::endl;
}

int PrintInfo(ExitReason reason, const LC3_VM& vm)
{
    switch (reason)
    {
        case ExitReason::HALTED:
            std::cout << "[Info] Simulation finished." << std::endl;
            vm.PrintReg();
            std::cout << "Instr Count: " << vm.GetInstrCount() << std::endl;
            return 0;

        case ExitReason::TIMEOUT:
            std::cout << "[Error] Instruction limit exceeded (Timeout)." << std::endl;
            return 1;

        case ExitReason::BREAKPOINT:
            std::cout << "[Debug] Breakpoint hit at x"
                    << std::hex << std::uppercase << vm.GetReg(PC) << std::dec << std::endl;
            return 0;

        default:
            std::cout << "[Error] Runtime exception occurred." << std::endl;
            return 1;
    }
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
    bool debug_mode = false;

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

            if (arg == "-pc")
            {
                if (i + 1 >= argc)
                    throw std::invalid_argument("Missing args for -pc");

                uint16_t start_addr = static_cast<uint16_t>(Utils::Convert::StrToInt(argv[++i]));
                vm.SetPCStart(start_addr);
            }
            else if (arg == "-reg")
            {
                if (i + 2 >= argc)
                    throw std::invalid_argument("Missing args for -reg");

                int id = static_cast<int>(Utils::Convert::StrToInt(argv[++i]));
                uint16_t val = static_cast<uint16_t>(Utils::Convert::StrToInt(argv[++i]));

                if (id >= REG_R0 && id <= PC)
                    vm.SetReg(static_cast<RegID> (id), val);
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
            else if (arg == "-d")
                debug_mode = true;
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
    if (debug_mode)
    {
        // 交互式 debug 模式
        std::cout << "-- -Interactive Debugger- --" << std::endl;
        std::cout << "Commands: [s]tep, [c]ontinue, [p]rint_regs, [m]em <addr>, [q]uit" << std::endl;

        char cmd;
        while (std::cout << "(lc3) " && std::cin >> cmd)
        {
            switch(cmd)
            {
                case 'q':
                    return 0;

                case 's':
                {
                    bool ok = vm.Step();
                    vm.PrintReg();
                    if (!ok)
                        std::cout << "[Warning] Cannot step." << std::endl;
                    break;
                }

                case 'c':
                    PrintInfo(vm.Run(), vm);
                    break;

                case 'p':
                    vm.PrintReg();
                    break;
                
                case 'm':
                {
                    string addr_str;
                    std::cin >> addr_str;
                    try
                    {
                        auto addr = static_cast<uint16_t>(Utils::Convert::StrToInt(addr_str));
                        vm.PrintMem(addr, addr);
                    }
                    catch (...)
                    {
                        std::cout << "[Error] Invalid address format." << std::endl;
                    }
                    break;
                }

                default:
                    std::cout << "Unknown command" << std::endl;
                    std::cout << "Commands: [s]tep, [c]ontinue, [p]rint_regs, [m]em <addr>, [q]uit" << std::endl;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
    else
    {
        // 正常运行模式
        ExitReason reason = vm.Run();
        vm.PrintReg();
        std::cout << "Instr Count: " << vm.GetInstrCount() << std::endl;
        
        if (reason == ExitReason::BREAKPOINT || reason == ExitReason::HALTED)
            return 0;
        return 1;
    }

    return 0;
}