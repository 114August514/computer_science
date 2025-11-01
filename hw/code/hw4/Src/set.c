//
// Created by Admin on 25-10-28.
//

#include <stdio.h>
#include <stdlib.h>
#include "set.h"

static bool is_contained_in_node(const Set set, int index, int value)
{
    if (index < 0 || index >= set.size) return false;

    SetNode* current = set.node_array[index];
    while (current != NULL)
    {
        if (current->value == value) return true;
        current = current->next;
    }

    return false;
}

static void free_node(SetNode* node)
{
    SetNode* current = node;
    while (current != NULL)
    {
        SetNode* temp = current;
        current = current->next;
        free(temp);
    }
}

Set* create_set(int size)
{
    if (size <= 0) return NULL;

    Set* set = (Set*) malloc(sizeof(Set));
    if (!set) return NULL;

    set->size = size;
    set->node_array = (SetNode**) calloc(size, sizeof(SetNode*));
    if (!set->node_array)
    {
        free(set);
        return NULL;
    }

    return set;
}

bool is_contained(const Set set, int value)
{
    if (set.size <= 0 || set.node_array == NULL) return false;

    int index = value % set.size;
    if (index < 0) index += set.size;
    return is_contained_in_node(set, index, value);
}

void insert_node(Set* set, int value)
{
    if (set == NULL || set->size <= 0 || set->node_array == NULL) return;

    int index = value % set->size;
    if (index < 0) index += set->size;
    if (is_contained_in_node(*set, index, value)) return;

    SetNode* new_node = (SetNode*) malloc(sizeof(SetNode));
    if (!new_node) exit(1);
    new_node->value = value;
    new_node->next = set->node_array[index];
    set->node_array[index] = new_node;
}

void free_set(Set* set)
{
    if (set == NULL) return;
    for (int i = 0; i < set->size; i++)
    {
        free_node(set->node_array[i]);
    }

    free(set->node_array);
    free(set);
}

bool is_unique(int m, int n, const int arr[m][n])
{
    Set* set = create_set(m * n);
    if (set == NULL) exit(1);

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (is_contained(*set, arr[i][j]))
            {
                free_set(set);
                printf("no\n");
                return false;
            }

            insert_node(set, arr[i][j]);
        }
    }

    free_set(set);
    printf("yes\n");
    return true;
}