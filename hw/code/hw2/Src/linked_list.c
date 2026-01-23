//
// Created by Admin on 25-10-9.
//

#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

void array2list(const int* arr, const int n, Point* head)
{
    // 尾插法
    // head本身无意义，只是为了方便操作
    Point* current = head;
    for (int i = 0; i < n; i++)
    {
        Point* newNode = (Point*)malloc(sizeof(Point));
        newNode->value = arr[i];
        newNode->next = NULL;
        current->next = newNode;
        current = newNode;
    }
}

void printList(const Point* head)
{
    const Point* current = head->next; // 跳过头节点
    while (current != NULL)
    {
        printf("%d -> ", current->value);
        current = current->next;
    }
    printf("NULL\n");
}

void freeList(Point* head)
{
    // 从头节点的下一节点开始释放
    Point* current = head->next;
    while (current != NULL)
    {
        Point* temp = current;
        current = current->next;
        free(temp);
    }

    // 释放后将头节点的next设为NULL
    head->next = NULL;
}

void mergeList(Point* head1, Point* head2, Point* merged_head)
{
    // 预设head1和head2都是升序排列的
    // merged_head 要求是降序排列（故采用头插法）
    // 注意：链表的头结点都是哑元（不存储实际数据，仅用于简化操作）
    Point* p1 = head1->next;
    Point* p2 = head2->next;
    Point* head = merged_head;
    Point* temp = NULL;
    head->next = NULL;

    while (p1 != NULL && p2 != NULL)
    {
        if (p1->value < p2->value)
        {
            temp = p1->next;
            p1->next = head->next;
            head->next = p1;
            p1 = temp;
        }
        else
        {
            temp = p2->next;
            p2->next = head->next;
            head->next = p2;
            p2 = temp;
        }
    }

    // 处理剩余节点
    Point* p = (p1==NULL) ? p2 : p1;
    while (p != NULL)
    {
        temp = p->next;
        p->next = head->next;
        head->next = p;
        p = temp;
    }

    // 安全行为
    head1->next = NULL;
    head2->next = NULL;
}

void reverseList(Point* head)
{
    // 链表的头结点依旧是哑元
    Point* past = NULL;
    Point* current = head->next;
    Point* future = NULL;

    while (current != NULL)
    {
        future = current->next;
        current->next = past;
        past = current;
        current = future;
    }

    head->next = past;
}

bool hasCycle(const Point* head)
{
    if (head == NULL)
        return false;

    Point* slow = head;
    Point* fast = head;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
            return true;
    }

    return false;
}