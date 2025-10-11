//
// Created by Admin on 25-10-9.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ackermann.h"

void initStack(Stack* stack)
{
    if (stack == NULL) return;
    stack->top = NULL;
    stack->size = 0;
}

bool isStackEmpty(const Stack* stack)
{
    if (stack == NULL) return true;
    return stack->size == 0;
}

void push(Stack* stack, int m, int n)
{
    StackFrameHandler* node = malloc(sizeof(StackFrameHandler));
    if (node == NULL)
    {
        printf("内存满了，开不动了。");
        exit(1);
    }
    node->m = m;
    node->n = n;
    node->result = -1;
    node->state = START;
    node->next = stack->top;
    stack->top = node;
    stack->size++;
}

StackFrameHandler* pop(Stack* stack)
{
    if (isStackEmpty(stack)) return NULL;
    StackFrameHandler* top_frame = stack->top;
    stack->top = stack->top->next;
    stack->size--;
    return top_frame;
}

void freeStack(Stack* stack)
{
    while (!isStackEmpty(stack))
    {
        StackFrameHandler* temp = pop(stack);
        free(temp);
    }
}

int ack(const int m, const int n)
{
    if (m == 0) return n + 1;
    if (n == 0) return ack(m - 1, 1);
    return ack(m - 1, ack(m, n - 1));
}

int ackWithStack(const int m, const int n)
{
    // 初始化阶段
    Stack* stack = malloc(sizeof(Stack));
    int result = -1;
    initStack(stack);

    // 正式处理阶段
    push(stack, m, n);
    while (!isStackEmpty(stack))
    {
        StackFrameHandler* current = stack->top;
        switch (current->state)
        {
            case START:
                if (current->m == 0)
                {
                    current->result = current->n + 1;
                    current->state = END;
                }
                else if (current->n == 0)
                {
                    current->m -= 1;
                    current->n = 1;
                }
                else 
                {
                    push(stack, current->m, current->n - 1);
                    current->state = WAITING;
                }
                break;

            case WAITING: // 进WAITING说明 (m != 0 && n != 0)
                assert(result != -1); // 保证结果如我预期

                current->m -= 1;
                current->n = result;
                current->state = START;
                result = -1;
                break;

            case END:
                result = current->result;
                pop(stack);
                break;
        }
    }

    // 结束收尾阶段
    freeStack(stack);
    free(stack);
    return result;
}
