//
// Created by Admin on 25-10-9.
//

#ifndef ACKERMANN_H
#define ACKERMANN_H

#include <stdbool.h>

typedef enum
{
    START = 0,
    WAITING,
    END
} StackState;

typedef struct StackFrame // 栈帧（节点信息）
{
    int m;
    int n;
    int result; // 初始值为-1
    StackState state;
    struct StackFrame* next;
} StackFrameHandler;

typedef struct
{
    StackFrameHandler* top;
    int size;
} Stack;

// 栈操作函数
void initStack(Stack* stack);
bool isStackEmpty(const Stack* stack);
void push(Stack* stack, int m, int n);
StackFrameHandler* pop(Stack* stack);
void freeStack(Stack* stack);

// ack(m, n)
int ack(const int m, const int n);
int ackWithStack(const int m, const int n);

#endif //ACKERMANN_H
