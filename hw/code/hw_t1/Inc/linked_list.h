//
// Created by Admin on 26-1-9.
//

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

// 数据类型定义
typedef int ElementType;

typedef struct Node
{
    ElementType data;
    struct Node* next;
} Node;

typedef struct 
{
    Node* head;
    Node* tail;
    int size;
} CircularList;

// 接口声明：
// 1. 初始化：包含哑元节点
CircularList* List_Init();

// 2. 插入
bool Insert(Node* prev_node, ElementType val, CircularList* list);

// 3. 删除
bool Delete(Node* prev_node, CircularList* list);

// 4. 查找
Node* Find(CircularList* list, ElementType val);

// 5. 遍历打印
void Print(const CircularList* list);

// 6. 判空
bool IsEmpty(const CircularList* list);

// 7. 销毁
void Destory(CircularList* list);


#endif //LINKED_LIST_H
