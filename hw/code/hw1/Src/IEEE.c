//
// Created by Admin on 25-10-15.
//

#include "IEEE.h"
#include <math.h>

static void print_binary(uint32_t num)
{
    for (int i = 31; i >= 0; i--)
    {
        uint32_t mask = 1u << i;
        putchar((num & mask) ? '1' : '0');

        if (i > 0 && i % 4 == 0) putchar(' ');
    }
}

bool is_equal(Num number)
{
    // 粗测（在大数时，只保证二进制一样，而实际从二进制转换回来后就不一样了）
    bool values_are_equal = (number.f_num == number.int_num);
    return values_are_equal;
}

void check_float_equal_int()
{
    for (uint64_t i = 0; i <= 0xFFFFFFFF; i++)
    {
        Num current_num;
        current_num.original = (uint32_t)i;

        if (is_equal(current_num))
        {
            printf("%d is allowed\n", current_num.int_num);

            uint32_t int_bits = int2complement(current_num.int_num);
            uint32_t float_bits = float2complement(current_num.f_num);

            // 打印整数的二进制补码
            printf("int_complement:   ");
            print_binary(int_bits);
            printf("\n");

            // 打印浮点数值对应的位模式
            printf("float_complement: ");
            print_binary(float_bits);
            printf("\n\n"); // 添加一个空行以分隔结果
        }
    }
    printf("over!\n");
}

uint32_t int2complement(int num)
{
    return (uint32_t)num;
}

uint32_t float2complement(float num)
{
    Num temp;
    temp.f_num = num;
    return temp.original;
}