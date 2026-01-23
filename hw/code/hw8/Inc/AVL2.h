//
// Created by Admin on 25-12-10.
//

#ifndef AVL2_H
#define AVL2_H

// 定义数据类型
typedef int KeyType;
typedef struct AVLNode2
{
    KeyType key;            // 节点关键字
    struct AVLNode2* left;   // 左子节点
    struct AVLNode2* right;  // 右子节点
    int b;                  // 平衡因子
} AVLNode2;

AVLNode2* AVL2_insert(AVLNode2* root, KeyType key);
AVLNode2* AVL2_delete(AVLNode2* root, KeyType key);
AVLNode2* AVL2_search(AVLNode2* root, KeyType key);

void AVL2_print(AVLNode2* root);
void AVL2_free(AVLNode2* root);

int get_height(const AVLNode2* root);

#endif //AVL2_H
