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
    int height;             // 树高
} AVLNode;

AVLNode* AVL_insert(AVLNode* root, KeyType key);
AVLNode* AVL_delete(AVLNode* root, KeyType key);
AVLNode* AVL_search(AVLNode* root, KeyType key);

void AVL_print(AVLNode* root);
void AVL_free(AVLNode* root);
int AVL_get_height(AVLNode* root);

#endif //AVL_H
