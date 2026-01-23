//
// Created by Admin on 25-12-10.
//

#include <stdio.h>
#include <stdlib.h>
#include "AVL.h"

// 辅助函数
static int max(int a, int b)
{
    return (a > b) ? a : b;
}

static AVLNode* new_node(KeyType key)
{
    AVLNode* node = (AVLNode*)calloc(1, sizeof(AVLNode));
    node->key = key;
    node->height = 1;
    return node;
}

static int height(const AVLNode* root)
{
    if (root == NULL) return 0;
    return root->height;
}

static int get_balance(const AVLNode* root)
{
    if (root == NULL) return 0;
    return height(root->left) - height(root->right);
}

static void update(AVLNode* root)
{
    if (root == NULL) return;
    root->height = max(height(root->left), height(root->right)) + 1;
}

// 旋转操作
static AVLNode* left_rotate(AVLNode* node)
{
    AVLNode* current = node->right;
    AVLNode* branch = current->left;

    current->left = node;
    node->right = branch;

    update(node);
    update(current);
    return current;
}

static AVLNode* right_rotate(AVLNode* node)
{
    AVLNode* current = node->left;
    AVLNode* branch = current->right;

    current->right = node;
    node->left = branch;

    update(node);
    update(current);
    return current;
}

// 找到子树的最小节点（默认使用右子树的最小节点实现双子节点的删除）
static AVLNode* min_value_node(AVLNode* node)
{
    AVLNode* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

AVLNode* AVL_insert(AVLNode* root, KeyType key)
{
    if (root == NULL) return new_node(key);

    if (key < root->key)
        root->left = AVL_insert(root->left, key);
    else if (key > root->key)
        root->right = AVL_insert(root->right, key);
    else return root;

    update(root);
    int balance = get_balance(root);

    // 判断是否失衡
    if (balance > 1) // LX case
    {
        if (key > root->left->key) // LR case
            root->left = left_rotate(root->left);
        return right_rotate(root); // LL/LR 共用
    }
    else if (balance < -1) // RX case
    {
        if (key < root->right->key) // RL case
            root->right = right_rotate(root->right);
        return left_rotate(root); // RR/RL 共用
    }
    return root;
}

AVLNode* AVL_delete(AVLNode* root, KeyType key)
{

}