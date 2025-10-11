//
// Created by Admin on 25-10-9.
//

#include "stack.h"

bool isHandlerAllowed(const char handler[], const int n)
{
    // n 输入之前记得不要计算字符串最后的"\0"
    if (n < 0 || n % 2 == 1) return false;

    int stack_state = 0;
    for (int i = 0; i < n; i++)
    {
        if (handler[i] == 'I') stack_state++;
        else if (handler[i] == 'O') stack_state--;

        if (stack_state < 0) return false;
    }

    if (stack_state != 0) return false;
    return true;
}