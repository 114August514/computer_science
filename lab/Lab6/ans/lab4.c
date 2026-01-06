// 原谅我没有分出来 stack.h stack.c 实现
// printf 单纯用以测试，不干扰其他代码的正常功能
// 预设 0 ≤ N ≤ 5 且 0 ≤ M ≤ 5

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_STACK_SIZE 100

typedef enum
{
    START = 0,
    FINISHED_1st_RECURSE,
    FINISHED_2nd_RECURSE
} STATE;

typedef struct
{
    // 栈帧
    int16_t input_n;
    int16_t input_m;

    int16_t temp; // 用于存储 ROUTES(i-1, j)

    STATE state;
} Frame;

typedef struct 
{
    int16_t top;
    Frame frame[MAX_STACK_SIZE];
} STACK;

static void push(STACK* stack, int16_t input_n, int16_t input_m)
{
    int16_t top = ++stack->top;
    if (top >= MAX_STACK_SIZE)
    {
        stack->top--;
        return;
    }

    stack->frame[top].input_n = input_n;
    stack->frame[top].input_m = input_m;
    stack->frame[top].state = START;
}

static void push_temp(STACK* stack, int16_t val)
{
    int16_t top = stack->top;
    stack->frame[top].temp = val;
    stack->frame[top].state = FINISHED_2nd_RECURSE;
}

static void pop(STACK* stack)
{
    if (stack->top == -1)
        return;

    stack->top--;
}

static int16_t cal_steps(int16_t N, int16_t M)
{
    return N + M;
}

static int16_t cal_routes(int16_t N, int16_t M)
{
    STACK* stack = malloc(sizeof(STACK));
    stack->top = -1;
    int16_t return_val;

    push(stack, N, M);
    while (stack->top != -1)
    {
        int16_t top = stack->top;
        int16_t current_n = stack->frame[top].input_n;
        int16_t current_m = stack->frame[top].input_m;

        switch (stack->frame[top].state)
        {
            case START:
                if (current_n == 0 || current_m == 0)
                {
                    return_val = 1;
                    pop(stack);
                }
                else
                {
                    stack->frame[top].state = FINISHED_1st_RECURSE;
                    push(stack, current_n - 1, current_m);
                }
                break;

            case FINISHED_1st_RECURSE:
                push_temp(stack, return_val);
                push(stack, current_n, current_m - 1);
                break;
            
            case FINISHED_2nd_RECURSE:
                return_val = return_val + stack->frame[top].temp;
                pop(stack);
                break;
        }
    }

    return return_val;
}

static int16_t cal_rec(int16_t N, int16_t M)
{
    int16_t rec = 0;
    int16_t steps = cal_steps(N, M);
    int16_t routes = cal_routes(N, M);

    for (int i = 0; i < 5; i++)
        rec += routes;

    rec -= steps;
    return rec;
}

static int16_t cal_routes2(int16_t N, int16_t M)
{
    if (N == 0 || M == 0) return 1;
    return cal_routes2(N - 1, M) + cal_routes2(N, M - 1);
}

static int16_t cal_rec2(int16_t N, int16_t M)
{
    int16_t rec = 0;
    int16_t steps = cal_steps(N, M);
    int16_t routes = cal_routes2(N, M);

    for (int i = 0; i < 5; i++)
        rec += routes;

    rec -= steps;
    return rec;
}

static void test_cal()
{
    for (int i = 0; i <= 5; i++)
        for (int j = 0; j <= 5; j++)
            printf("N = %d, M = %d, %d / %d\n", i, j, cal_rec(i, j), cal_rec2(i, j));
}

int main()
{
    test_cal();
    return 0;
}