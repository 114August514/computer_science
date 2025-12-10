//
// Created by Admin on 25-12-10.
//

#include <stdio.h>
#include "AVL.h"

int get_height(AVLNode* root)
{
    if (root == NULL) return 0;
    if (root->b > 0) return get_height(root->left) + 1;
    return get_height(root->right) + 1;
}