//
// Created by Admin on 25-10-9.
//

#include "stdio.h"
#include "stack.h"
#include "ackermann.h"
#include "linked_list.h"
#include "fun.h"
#include "main.h"

int main()
{
    enumerate_possiblity();

    const int a[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    const int num = sizeof(a) / sizeof(a[0]);
    Node head = {0, NULL};
    array2list(a, num, &head);
    printList(&head);
    printf("Max: %d\n", findMaxData(&head));
    printf("Count: %d\n", countNodesNum(&head));
    printf("Average: %.2f\n", averageData(&head));
    freeList(&head);

    const char arr[] = "IIIOOIOO";
    const int size = sizeof(arr) / sizeof(arr[0]) - 1; // 保证不计算末尾的'\0'

    if (isHandlerAllowed(arr, size))
        printf("Yes\n");
    else
        printf("No\n");

    int m = 2, n = 1;
    int result = ack(m, n);
    int result_with_stack = ackWithStack(m, n);
    printf("%d", result);
    printf("\n");
    printf("%d", result_with_stack);

    return 0;
}