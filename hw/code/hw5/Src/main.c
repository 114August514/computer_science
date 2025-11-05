//
// Created by Admin on 25-11-5.
//

#include <stdio.h>
#include <stdlib.h>
#include "binary_tree.h"

static TreeNode *build_from_string(const char** str_p)
{
    if (str_p == NULL || *str_p == NULL) return NULL;

    char c = **str_p;
    if (c == '\0') return NULL;

    (*str_p)++;

    if (c == '#') return NULL;
    TreeNode* T = createNode(c - '0');
    if (T == NULL) return NULL;

    T->lchild = build_from_string(str_p);
    T->rchild = build_from_string(str_p);
    return T;
}

static TreeNode* str2tree(const char str[])
{
    const char* p = str;
    return build_from_string(&p);
}

int main(void)
{
    char str1[] = "ABDH###E##CFJ###G##";
    char str2[] = "";

    TreeNode* T1 = str2tree(str1);
    TreeNode* T2 = str2tree(str2);

    if (isEqual(T1, T2)) printf("111\n");
    else printf("222\n");

    printf("%d\n", getMaxWidth(T2));

    return 0;
}
