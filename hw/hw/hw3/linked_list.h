//
// Created by Admin on 25-10-9.
//

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node 
{
    int data;
    struct Node* next;
} Node;

typedef struct
{
    int count;
    int sum;
} Analysis;

void array2list(const int* arr, const int n, Node* head);
void printList(const Node* head);
void freeList(Node* head);

int findMaxData(const Node* head);
int countNodesNum(const Node* head);
double averageData(const Node* head);

#endif //LINKED_LIST_H
