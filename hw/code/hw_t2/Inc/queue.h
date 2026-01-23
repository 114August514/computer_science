//
// Created by Admin on 26-1-9.
//

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef int ElementType;

typedef struct QueueNode
{
    ElementType value;
    struct QueueNode* next;
} QueueNode;

typedef struct 
{
    QueueNode* front;
    QueueNode* rear;
    int size;    
} Queue;

Queue* createQueue();
void enqueue(Queue* queue, ElementType value);
ElementType dequeue(Queue* queue);
ElementType peekFront(Queue* queue);
bool isQueueEmpty(Queue* queue);
int getQueueSize(Queue* queue);
void freeQueue(Queue* queue);

#endif //QUEUE_H
