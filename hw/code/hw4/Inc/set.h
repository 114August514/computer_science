//
// Created by Admin on 25-10-28.
//

#ifndef SET_H
#define SET_H

#include <stdbool.h>

typedef struct Node
{
    int value;
    struct Node* next;
} SetNode;

typedef struct
{
    SetNode** node_array;
    int size;
} Set;

Set* create_set(int size);
bool is_contained(const Set set, int value);
void insert_node(Set* set, int value);
void free_set(Set* set);

bool is_unique(int m, int n, const int arr[m][n]);

#endif //SET_H
