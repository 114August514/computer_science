//
// Created by Admin on 26-1-9.
//

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

typedef int ElemType;

typedef struct TreeNode
{
    ElemType value;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* createNode(ElemType value);
TreeNode* insertLeft(TreeNode* parent, ElemType value);
TreeNode* insertRight(TreeNode* parent, ElemType value);
void freeTree(TreeNode* root);
void preOrderTraversal(TreeNode* root);
void inOrderTraversal(TreeNode* root);
void postOrderTraversal(TreeNode* root);

#endif //BINARY_TREE_H
