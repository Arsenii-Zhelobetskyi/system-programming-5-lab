#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
    printf("Я є другою програмою(%s) \n", argv[1]);
    for (int i=0; envp[i]!=NULL; i++)
        printf("%s\n", envp[i]);
    return 0;
}