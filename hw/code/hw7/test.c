#include <stdio.h>
struct Test
{
    char c[3];
    int i;
} t;

int main()
{
    scanf("%s", t.c);
    printf("%d\n", t.i);
    return 0;
}