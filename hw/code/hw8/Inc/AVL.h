//
// Created by Admin on 25-12-10.
//

#ifndef AVL_H
#define AVL_H

// 定义数据类型
typedef int KeyType;
typedef struct AVLNode
{
    KeyType key;            // 节点关键字
    struct AVLNode* left;   // 左子节点
    struct AVLNode* right;  // 右子节点
    int b; // 平衡因子
} AVLNode;

int get_height(AVLNode* root);

#endif //AVL_H
