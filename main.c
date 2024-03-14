#include <stdio.h>
#include <sys/types.h> // CHECK
#include <sys/wait.h> // for wait
#include <unistd.h> // for fork
#include <stdlib.h> // for exit
int main() {
    pid_t process = fork();
    if (process < 0) {
        printf("Невдалось створити дочірній процес\n");
    } else if (process == 0) {
        printf("Дочірній процес. Айді процесу = %d\n", getpid());
        exit(993);
    } else {
        printf("Батьківський процес. Айді процесу = %d\n", getpid());
        int status;
        wait(&status);
        if (WIFEXITED(status))
            printf("Дочірній процес завершився зі статусом %d\n", WEXITSTATUS(status));
        else
            printf("Дочірній процес завершився некоректно\n");

//            printf("Дочірній процес завершився зі статусом %d\n", WEXITSTATUS(status));
    }
    return 0;
}
