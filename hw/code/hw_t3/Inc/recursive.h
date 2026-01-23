//
// Created by Admin on 26-1-9.
//

#ifndef RECURSIVE_H
#define RECURSIVE_H

typedef enum
{
    Start,
    FinishedFirstRecurse
} State;

typedef struct Frame
{
    int n;
    char from;
    char to;
    char temp;
    State state;
    Frame* next;
} Frame;

typedef struct 
{
    Frame* head;
    int size;
} Stack;

void push(Stack* stack, Frame frame);
void pop(Stack* stack);

int recursive_function(int n);
int recursive_function2(int n);
int fibonacci(int n);
int fibonacci2(int n);
void hanoi(int n, char from, char to, char temp);
void hanoi2(int n, char from, char to, char temp);

#endif //RECURSIVE_H
