//
// Created by Admin on 25-10-9.
//

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "fun.h"

static char signal[3] = {' ', '+', '-'};

static void print_array(const char* str)
{
    const char* temp = str;
    while (*temp != '\0')
    {
        if (*temp == ' ');
        else printf("%c", *temp);

        temp++;
    }

    printf("\n");
}

static int calculator(const char* str)
{
    const char* temp = str;
    int num = 0;
    int sum = 0;
    int sign = 1; // 1表示正；-1表示负

    while (*temp != '\0')
    {
        if (*temp == ' ');
        else if (isdigit(*temp)) num = num * 10 + (*temp - '0'); 
        else if (*temp == '+' || *temp == '-')
        {
            sum += sign * num;
            num = 0;
            sign = (*temp == '+') ? 1 : -1; 
        }

        temp++;
    }

    sum += sign * num;
    return sum;
}

static void generate(char* str, int key)
{
    // key 是从 0 到 3**8 - 1
    char* current = str + 1;
    int index = 0;
    while (*current != '\0')
    {
        index = key % 3;
        *current = signal[index];
        current += 2;
        key /= 3;
    }
}

void enumerate_possiblity()
{
    char original[] = "1 2 3 4 5 6 7 8 9";
    for (int i = 0; i < pow(3, 8); i++)
    {
        generate(original, i);
        if (calculator(original) == 110) print_array(original);
    }
}