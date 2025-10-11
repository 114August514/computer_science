//
// Created by Admin on 25-10-9.
//

#include <stdio.h>
#include "linked_list.h"
#include "main.h"

int main()
{
    int arr1[] = {1, 2, 4, 7, 9};
    int arr2[] = {2, 4, 6, 8, 10};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    Point head1 = {0, NULL};
    Point head2 = {0, NULL};
    Point merged_head = {0, NULL};
    array2list(arr1, n1, &head1);
    array2list(arr2, n2, &head2);

    printList(&head1);
    printList(&head2);

    mergeList(&head1, &head2, &merged_head);
    printList(&merged_head);
    reverseList(&merged_head);
    printList(&merged_head);

    freeList(&head1);
    freeList(&head2);
    freeList(&merged_head);
    return 0;
}