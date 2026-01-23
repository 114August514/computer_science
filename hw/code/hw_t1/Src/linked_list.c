//
// Created by Admin on 26-1-9.
//

#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

CircularList* List_Init()
{
    CircularList* new = (CircularList*) calloc(1, sizeof(CircularList));
    if (new == NULL) return NULL;

    Node* current = malloc(sizeof(Node));
    if (current == NULL)
    {
        free(new);
        return NULL;
    }

    current->next = current; // 哑元节点

    new->head = current;
    new->tail = current;
    return new;
}

bool Insert(Node* prev_node, ElementType val, CircularList* list)
{
    if (list == NULL || prev_node == NULL)
        return false;

    Node* current = malloc(sizeof(Node));
    if (current == NULL)
        return false;

    current->data = val;
    current->next = prev_node->next;
    prev_node->next = current;

    // 维护 tail 指针
    if (prev_node == list->tail)
        list->tail = current;

    list->size++;
    return true;
}

bool Delete(Node* prev_node, CircularList* list)
{
    if (list == NULL || list->size == 0 || prev_node->next == list->head)
        return false;

    Node* temp = prev_node->next;

    prev_node->next = temp->next;
    
    if (temp == list->tail)
        list->tail = prev_node;

    free(temp);
    list->size--;
    return true;
}

Node* Find(CircularList* list, ElementType val)
{
    if (list == NULL)
        return NULL;

    Node* current = list->head->next;
    
    while (current != list->head)
    {
        if (val == current->data)
            return current;

        current = current->next;
    }

    return NULL;
}

void Print(const CircularList* list)
{
    if (list == NULL)
        return;

    Node* current = list->head->next;
    int size = 1;

    while (current != list->head)
    {
        printf("%d : %d", size++, current->data);
        current = current->next;
    }
}

bool IsEmpty(const CircularList* list)
{
    if (list == NULL || list->size == 0)
        return true;

    return false;
}

void Destory(CircularList* list)
{
    if (list == NULL)
        return;

    Node* current = list->head->next;

    while (current != list->head)
    {
        Node* temp = current;
        current = current->next;
        free(temp);
    }

    free(current);
    free(list);
}