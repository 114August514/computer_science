//
// Created by Admin on 26-1-9.
//

#include <stdio.h>
#include <stdlib.h>
#include "binary_tree.h"

TreeNode* createNode(ElemType value)
{
    TreeNode* curr = calloc(1, sizeof(TreeNode));
    if (curr == NULL) return NULL;
    curr->value = value;

    return curr;
}

TreeNode* insertLeft(TreeNode* parent, ElemType value)
{
    if (parent == NULL) return NULL;
    TreeNode* left = createNode(value);
    parent->left = left;
    return parent;
}

TreeNode* insertRight(TreeNode* parent, ElemType value)
{
    if (parent == NULL) return NULL;
    TreeNode* right = createNode(value);
    parent->right = right;
    return parent;
}

void freeTree(TreeNode* root)
{
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void preOrderTraversal(TreeNode* root)
{
    if (root == NULL) return;

    printf("%d -> ", root->value);
    preOrderTraversal(root->left);
    preOrderTraversal(root->right);
}

void inOrderTraversal(TreeNode* root)
{
    if (root == NULL) return;

    inOrderTraversal(root->left);
    printf("%d -> ", root->value);
    inOrderTraversal(root->right);
}

void postOrderTraversal(TreeNode* root)
{
    if (root == NULL) return;

    postOrderTraversal(root->left);
    postOrderTraversal(root->right);
    printf("%d -> ", root->value);
}