//
// Created by Admin on 26-1-9.
//

#include <limits.h>
#include <stdlib.h>
#include "stack.h"

Stack* createStack()
{
    Stack* stack = (Stack*) calloc(1, sizeof(Stack));
    if (stack == NULL)
        return NULL;
    return stack;
}

void push(Stack* stack, ElementType value)
{
    if (stack == NULL) return;
    StackNode* current = malloc(sizeof(StackNode));
    if (current == NULL) return;
    
    current->next = stack->top;
    current->value = value;

    stack->top = current;
    stack->size++;
}

ElementType pop(Stack* stack)
{
    if (stack == NULL || stack->size <= 0)
        return INT_MIN;

    StackNode* current = stack->top;
    ElementType val = current->value;

    stack->top = current->next;
    stack->size--;
    free(current);

    return val;
}

ElementType peek(Stack* stack)
{
    if (stack == NULL || stack->size <= 0)
        return INT_MIN;

    return stack->top->value;
}

bool isEmpty(Stack* stack)
{
    if (stack == NULL || stack->size <= 0)
        return true;

    return false;
}

int getSize(Stack* stack)
{
    if (stack == NULL)
        return INT_MIN;
    
    return stack->size;
}

void freeStack(Stack* stack)
{
    if (stack == NULL)
        return;

    StackNode* current = stack->top;

    while (current != NULL)
    {
        StackNode* temp = current;
        current = current->next;

        free(temp);
    }

    free(stack);
}