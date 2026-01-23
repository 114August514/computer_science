//
// Created by Admin on 25-10-9.
//

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

typedef int ElementType;

typedef struct Node {
    ElementType value;
    struct Node* next;
} Point;

void array2list(const int* arr, const int n, Point* head);
void printList(const Point* head);
void freeList(Point* head);

void mergeList(Point* head1, Point* head2, Point* merged_head);
void reverseList(Point* head);

bool hasCycle(const Point* head);

#endif //LINKED_LIST_H
