//
// Created by Admin on 25-11-5.
//

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdbool.h>

typedef int ElemType;

typedef struct TreeNode 
{
    ElemType data;
    struct TreeNode *lchild, *rchild;
} TreeNode;

typedef struct QueueNode
{
    TreeNode* treenode;
    struct QueueNode* next;
} QueueNode;

typedef struct
{
    QueueNode* front;
    QueueNode* rear;
} Queue;

TreeNode* createNode(ElemType data); // 创造新节点
bool isEqual(const TreeNode* t1, const TreeNode* t2); // 是否相等
void clear(TreeNode* root); // 销毁树

Queue* createQueue();
bool emptyQueue(const Queue* q);
void push(Queue* q, TreeNode* treenode);
TreeNode* pop(Queue* q);
void clearQueue(Queue* q);
void destoyQueue(Queue* q);

int getMaxWidth(TreeNode* root);

#endif //BINARY_TREE_H
