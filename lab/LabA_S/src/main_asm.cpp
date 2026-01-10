//
// Created by Admin on 26-1-4.
//

#include <vector>
#include <iostream>
#include <string>
#include "assembler.h"

void ShowUsage()
{
    std::cout << "Usage:" << std::endl;
    std::cout << "  Assemble: assembler [options] <input.asm> <output.bin>" << std::endl;
    std::cout << "   -d   Debug mode (print machine code)" << std::endl;
}

int main(int argc, char* argv[])
{
    std::vector<string> args;
    bool debug_mode = false;

    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];
        if (arg == "-d")
            debug_mode = true;
        else 
            args.push_back(arg);
    }

    if (args.size() != 2)
    {
        std::cout << "[Error] Invalid arguments count: " << args.size() << std::endl;
        ShowUsage();
        return 1;
    }

    string input_path = args[0];
    string output_path = args[1];

    Assembler assembler(input_path, output_path);

    std::cout << "Assembling '" << input_path << "' -> '" << output_path << "' ..." << std::endl;

    if (assembler.Run(debug_mode))
    {
        std::cout << "Success!" << std::endl;
        return 0;
    }
    
    std::cout << "Assembly Failed." << std::endl;
    return -1;
}