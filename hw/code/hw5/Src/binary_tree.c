//
// Created by Admin on 25-11-5.
//

#include <stdio.h>
#include <stdlib.h>
#include "binary_tree.h"

TreeNode* createNode(ElemType data)
{
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL)
    {
        printf("ERROR1!\n");
        return NULL;
    }
    newNode->data = data;
    newNode->lchild = NULL;
    newNode->rchild = NULL;
    return newNode;
}

bool isEqual(const TreeNode* t1, const TreeNode* t2)
{
    if (t1 == NULL && t2 == NULL) return true;
    if (t1 == NULL || t2 == NULL || t1->data != t2->data) return false;

    return isEqual(t1->lchild, t2->lchild) && isEqual(t1->rchild, t2->rchild);
}

void clear(TreeNode *root)
{
    if (root == NULL) return;

    clear(root->lchild);
    clear(root->rchild);
    free(root);
}

Queue* createQueue() // 含哑元
{
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL)
    {
        printf("ERROR2!\n");
        return NULL;
    }

    q->front = (QueueNode*)malloc(sizeof(QueueNode));
    if (q->front == NULL)
    {
        printf("ERROR2!\n");
        free(q);
        return NULL;
    }
    q->front->next = NULL;
    q->rear = q->front; // 尾节点与哑元重合，表示队列为空
    return q;
}

bool emptyQueue(const Queue* q)
{
    if (q == NULL)
    {
        printf("ElementNotFoundError!");
        return false;
    }

    return q->front == q->rear;
}

void push(Queue* q, TreeNode* treenode)
{
    if (q == NULL) 
    {
        printf("ElementNotFoundError!");
        return;
    }

    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL)
    {
        printf("ERROR3!\n");
        return;
    }
    newNode->treenode = treenode;
    newNode->next = NULL;

    q->rear->next = newNode;
    q->rear = newNode;
}

TreeNode* pop(Queue* q)
{
    if (q == NULL)
    {
        printf("ElementNotFoundError!");
        return NULL;
    }

    if (emptyQueue(q)) return NULL;

    QueueNode* tempNode = q->front->next;
    TreeNode* tempTree = tempNode->treenode;

    q->front->next = tempNode->next;
    if (q->rear == tempNode) q->rear = q->front;

    free(tempNode);
    return tempTree;
}

void clearQueue(Queue* q)
{
    if (q == NULL || q->front == NULL) return;
    while(!emptyQueue(q)) pop(q);
}

void destoyQueue(Queue *q)
{
    if (q == NULL) return;
    clearQueue(q);
    free(q->front);
    free(q);
}

int getMaxWidth(TreeNode *root)
{
    if (root == NULL) return 0;

    Queue* q = createQueue();
    push(q, root);
    int maxWidth = 0;
    while(!emptyQueue(q))
    {
        QueueNode* temp = q->front->next;
        int currentWidth = 0;
        while(temp != NULL)
        {
            currentWidth++;
            temp = temp->next;
        }

        if (currentWidth > maxWidth) maxWidth = currentWidth;

        for (int i = 0; i < currentWidth; i++)
        {
            TreeNode* node = pop(q);
            if (node->lchild != NULL) push(q, node->lchild);
            if (node->rchild != NULL) push(q, node->rchild);
        }
    }

    destoyQueue(q);
    return maxWidth;
}