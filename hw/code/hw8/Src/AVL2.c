//
// Created by Admin on 25-12-10.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "AVL2.h"

static AVLNode2* new_node(KeyType key)
{
    AVLNode2 *node = (AVLNode2*)calloc(1, sizeof(AVLNode2));
    node->key = key;
    return node;
}

// 仅进行旋转操作
static AVLNode2* left_rotate(AVLNode2* node)
{
    AVLNode2* current = node->right;
    AVLNode2* branch = current->left;

    current->left = node;
    node->right = branch;
    return current;
}

static AVLNode2* right_rotate(AVLNode2* node)
{
    AVLNode2* current = node->left;
    AVLNode2* branch = current->right;

    current->right = node;
    node->left = branch;
    return current;
}

// 调节 balance 值
static AVLNode2* left_balance(AVLNode2* root) // 处理 LX 情况
{
    AVLNode2* left = root->left;
    if (left->b == 1)
    {
        // LL Case
        root->b = 0;
        left->b = 0;
        root = right_rotate(root);
    }    
    else if (left->b == -1)
    {
        // 虽然只有两种情况，但以防万一
        // LR Case
        AVLNode2* lr = left->right;
        switch (lr->b)
        {
            case 1:
                root->b = -1;
                left->b = 0;
                break;

            case 0:
                root->b = 0;
                left->b = 0;
                break;

            case -1:
                root->b = 0;
                left->b = 1;
                break;
        }
        lr->b = 0;

        // 旋转
        root->left = left_rotate(left);
        root = right_rotate(root);
    }
    return root; // 保证 left->b == 0 时，安全退出（虽然应该不会有该情况）
}

static AVLNode2* right_balance(AVLNode2* root) // 处理 RX 情况
{
    AVLNode2* right = root->right;
    if (right->b == -1)
    {
        // RR Case
        root->b = 0;
        right->b = 0;
        root = left_rotate(root);
    }
    else if (right->b == 1)
    {
        // RL Case
        AVLNode2* rl = right->left;
        switch (rl->b)
        {
            case 1:
                root->b = 0;
                right->b = -1;
                break;

            case 0:
                root->b = 0;
                right->b = 0;
                break;

            case -1:
                root->b = 1;
                right->b = 0;
                break;
        }
        rl->b = 0;

        // 旋转
        root->right = right_rotate(right);
        root = left_rotate(root);
    }
    return root;
}

// 递归辅助函数
static AVLNode2* AVL2_insert_helper(AVLNode2* root, KeyType key, bool* taller)
{
    // taller 用以存储是否高度变化
    if (root == NULL)
    {
        // 退出条件
        root = new_node(key);
        *taller = true;
        return root;
    }

    if (root->key == key) *taller = false;
    else if (key < root->key)
    {
        // 插入左子树
        root->left = AVL2_insert_helper(root->left, key, taller);

        if (*taller) // 左子树变高则考虑调整
        {
            switch (root->b)
            {
                case 1:
                    root = left_balance(root);
                    *taller = false; // 调整之后高度不增加
                    break;

                case 0:
                    root->b = 1;
                    *taller = true;
                    break;

                case -1:
                    root->b = 0;
                    *taller = false;
                    break;
            }
        }
    }
    else
    {
        // 插入右子树
        root->right = AVL2_insert_helper(root->right, key, taller);

        if (*taller) // 右子树变高（考虑调整）
        {
            switch (root->b)
            {
                case 1:
                    root->b = 0;
                    *taller = false;
                    break;
                
                case 0:
                    root->b = -1;
                    *taller = true;
                    break;

                case -1:
                    root = right_balance(root);
                    *taller = false;
                    break;
            }
        }
    }

    return root;
}

AVLNode2* AVL2_insert(AVLNode2* root, KeyType key)
{
    bool taller = false;
    return AVL2_insert_helper(root, key, &taller);
}

int get_height(const AVLNode2 *root)
{
    if (root == NULL) return 0;
    if (root->b > 0) return get_height(root->left) + 1;
    return get_height(root->right) + 1;
}