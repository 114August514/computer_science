#include<stdio.h>
#include<string.h>
char s[11];
int main(){
    while(1){
        printf("/ 0 1 2\n"); fflush(stdout);
        scanf("%s", s);
        if(strlen(s) == 9) break; //only strlen("MathError")=9
        printf("- 1 0 1\n"); fflush(stdout);
        scanf("%s", s);
        if(strlen(s) == 9) break;
    }
    printf("!\n"); fflush(stdout);
    return 0;
}
