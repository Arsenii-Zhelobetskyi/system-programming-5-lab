#include <stdio.h>
#include <sys/types.h> // CHECK
#include <unistd.h> // for fork

int main() {
     pid_t process = fork();
     if(process<0){
          printf("Невдалось створити дочірній процес\n");
     }
     else if(process==0){
          printf("Дочірній процес. Айді процесу = %d\n", getpid());
     }
     else{
          printf("Батьківський процес. Айді процесу = %d\n", getpid());
     }
     return 0;
}
