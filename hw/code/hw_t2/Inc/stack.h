//
// Created by Admin on 26-1-9.
//

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

typedef int ElementType;

typedef struct StackNode
{
    ElementType value;
    struct StackNode* next;
} StackNode;

typedef struct
{
    StackNode* top;
    int size;
} Stack;

Stack* createStack();
void push(Stack* stack, ElementType value);
ElementType pop(Stack* stack);
ElementType peek(Stack* stack);
bool isEmpty(Stack* stack);
int getSize(Stack* stack);
void freeStack(Stack* stack);

#endif //STACK_H
