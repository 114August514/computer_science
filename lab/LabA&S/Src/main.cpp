//
// Created by Admin on 26-1-4.
//

#include <vector>
#include <iostream>
#include "assembler.h"

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
        return 1;

    string input_path = args[0];
    string output_path = args[1];

    Assembler assembler(input_path, output_path);

    if (assembler.Run(debug_mode))
    {
        std::cout << "Success!" << std::endl;
        return 0;
    }
    
    std::cout << "Assembly Failed." << std::endl;
    return -1;
}