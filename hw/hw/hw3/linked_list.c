//
// Created by Admin on 25-10-9.
//

#include  <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

static int findMax(const Node* node)
{
    if (node->next == NULL) return node->data;
    int max = findMax(node->next);
    return (node->data > max) ? node->data : max;
}

static int countNodes(const Node* node)
{
    if (node == NULL) return 0;
    return countNodes(node->next) + 1;
}

static Analysis averageAnalysis(const Node* node)
{
    Analysis temp = {0, 0};
    if (node == NULL) return temp;
    
    temp = averageAnalysis(node->next);
    temp.count++;
    temp.sum += node->data;

    return temp;
}

void array2list(const int* arr, const int n, Node* head)
{
    // 尾插法
    // head本身无意义，只是为了方便操作
    Node* current = head;
    for (int i = 0; i < n; i++)
    {
        Node* newNode = malloc(sizeof(Node));
        newNode->data = arr[i];
        newNode->next = NULL;
        current->next = newNode;
        current = newNode;
    }
}

void printList(const Node* head)
{
    const Node* current = head->next; // 跳过头节点
    while (current != NULL)
    {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

void freeList(Node* head)
{
    // 从头节点的下一节点开始释放
    Node* current = head->next;
    while (current != NULL)
    {
        Node* temp = current;
        current = current->next;
        free(temp);
    }

    // 释放后将头节点的next设为NULL
    head->next = NULL;
}

int findMaxData(const Node* head)
{
    Node* current = head->next; // 跳过头节点
    if (current == NULL)
    {
        printf("无法找到空链表的最大元！");
        exit(1);
    }

    int max = findMax(current);
    return max;
}

int countNodesNum(const Node *head)
{
    Node* current = head->next;
    return countNodes(current);
}

double averageData(const Node *head)
{
    Node* current = head->next;
    Analysis analysis = averageAnalysis(current);

    if (analysis.count == 0) return 0.0;
    
    double result = (double) analysis.sum / analysis.count;
    return result;
}