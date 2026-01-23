//
// Created by Admin on 26-1-9.
//

#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "recursive.h"

static void cpyframe(Frame* frame1, Frame frame2)
{
    frame1->from = frame2.from;
    frame1->n = frame2.n;
    frame1->temp = frame2.temp;
    frame1->to = frame2.to;
    frame1->state = frame2.state;
}

void push(Stack* stack, Frame frame)
{
    if (stack == NULL) return;
    Frame* curr = malloc(sizeof(Frame));
    if(curr == NULL)
        return;

    cpyframe(curr, frame);
    curr->next = stack->head;
    stack->head = curr;
    stack->size++;
}

void pop(Stack* stack)
{
    Frame* temp = stack->head;
    stack->head = temp->next;
    stack->size--;
    
    free(temp);
}

int recursive_function(int n)
{
    if (n <= 1) return 1;
    return n * recursive_function(n - 1);
}

int recursive_function2(int n)
{
    int sum = 1;
    while (n >= 1)
        sum *= n--;
    return sum;
}

int fibonacci(int n)
{
    if (n <= 2) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int fibonacci2(int n)
{
    if (n < 0) return INT_MIN;
    if (n <= 2) return 1;

    int temp1 = 1, temp2 = 1;
    for (int i = 3; i <= n; i++)
    {
        int result = temp1 + temp2;
        temp2 = temp1;
        temp1 = result;
    }

    return temp1;
}

void hanoi(int n, char from, char to, char temp)
{
    if (n <= 0)
    {
        printf("Error input!\n");
        return;
    }

    if (n == 1)
    {
        printf("Move %d from %c to %c.\n", n, from, to);
        return;
    }

    hanoi(n - 1, from, temp, to);
    printf("Move %d from %c to %c.\n", n, from, to);
    hanoi(n - 1, temp, to, from);
}

static void hanoi_helper(Stack* stack, int n, char from, char to, char temp)
{
    push(stack, (Frame){n, from, to, temp, Start});
    while (stack->size > 0)
    {
        Frame* current = stack->head;

        int curr_n = current->n;
        char curr_from = current->from;
        char curr_to = current->to;
        char curr_temp = current->temp;

        switch (current->state)
        {
            case Start:
            {
                if (curr_n == 1)
                {
                    printf("Move %d from %c to %c.\n", curr_n, curr_from, curr_to);
                    pop(stack);
                    break;
                }

                current->state = FinishedFirstRecurse;
                push(stack, (Frame){curr_n - 1, curr_from, curr_temp, curr_to, Start});
                break;
            }

            case FinishedFirstRecurse:
            {
                printf("Move %d from %c to %c.\n", curr_n, curr_from, curr_to);

                pop(stack);
                push(stack, (Frame){curr_n - 1, curr_temp, curr_to, curr_from, Start});
                break;
            }
        }
    }
}

void hanoi2(int n, char from, char to, char temp)
{
    if (n <= 0)
    {
        printf("Error input!\n");
        return;
    }

    Stack* stack = calloc(1, sizeof(Stack));
    if (stack == NULL)
    {
        printf("Error create stack!\n");
        return;
    }

    hanoi_helper(stack, n, from, to, temp);
    free(stack);
    printf("Success!\n");
}