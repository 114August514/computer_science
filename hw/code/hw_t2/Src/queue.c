//
// Created by Admin on 26-1-9.
//

#include <limits.h>
#include <stdlib.h>
#include "queue.h"

Queue* createQueue()
{
    Queue* queue = malloc(sizeof(Queue));
    if (queue == NULL) return NULL;

    QueueNode* head = calloc(1, sizeof(QueueNode));
    if (head == NULL)
    {
        free(queue);
        return NULL;
    }
    
    queue->front = head;
    queue->rear = head;
    queue->size = 0;

    return queue;
}

void enqueue(Queue* queue, ElementType value)
{
    if (queue == NULL) return;

    QueueNode* current = malloc(sizeof(QueueNode));
    if (current == NULL) return;

    current->value = value;
    current->next = NULL;

    queue->rear->next = current;
    queue->rear = current;
    queue->size++;
}

ElementType dequeue(Queue* queue)
{
    if (queue == NULL || queue->size <= 0)
        return INT_MIN;

    QueueNode* curr = queue->front->next;
    ElementType val = curr->value;
    
    queue->front->next = curr->next;
    queue->size--;

    if (curr == queue->rear)
        queue->rear = queue->front;

    free(curr);
    return val;
}

ElementType peekFront(Queue* queue)
{
    if (queue == NULL || queue->size <= 0)
        return INT_MIN;

    return queue->front->next->value;
}

bool isQueueEmpty(Queue* queue)
{
    if (queue == NULL || queue->size <= 0)
        return true;

    return false;
}

int getQueueSize(Queue* queue)
{
    if (queue == NULL)
        return INT_MIN;

    return queue->size;
}

void freeQueue(Queue* queue)
{
    if (queue == NULL)
        return;

    QueueNode* curr = queue->front;

    while (curr != NULL)
    {
        QueueNode* temp = curr;
        curr = curr->next;
        free(temp);
    }

    free(queue);
}