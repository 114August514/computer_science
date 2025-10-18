//
// Created by Admin on 25-10-15.
//

#ifndef IEEE_H
#define IEEE_H
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef union
{
    uint32_t original;
    int32_t int_num;
    float f_num;
} Num;

bool is_equal(Num number);
void check_float_equal_int();

uint32_t int2complement(int num);
uint32_t float2complement(float num);

#endif //IEEE_H
