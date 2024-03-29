
#include <stdio.h> // for printf
#include <sys/wait.h> // for wait
#include <unistd.h> // for fork
#include <stdlib.h> // for exit


#include <sys/ipc.h> // for ftok
#include <sys/shm.h> // for shmget
#include <string.h> // for strcpy

int main() {

    key_t key = ftok("shmfile", 65); // Генеруємо ключ для створення спільного розділу пам'яті
    // "shmfile" - це файл(унікальний рядок), який використовується для генерації ключа.
    // 65 - це ціле число, яке використовується для генерації ключа.

    int shmid = shmget(key, 1024,
                       0666 | IPC_CREAT);   // Створюємо або отримуємо ідентифікатор спільного розділу пам'яті
    // key - ключ, який ідентифікує розділ пам'яті. Цей ключ повинен бути унікальним для кожного спільного розділу пам'яті.
    // size - розмір розділу пам'яті, який ми хочемо створити
    // 0666 - це набір дозволів для спільного розділу пам'яті. Це дозволяє читати, писати та виконувати спільний розділ пам'яті.
    // IPC_CREAT - цей флаг вказує, що спільний розділ пам'яті повинен бути створений, якщо він не існує.

    pid_t process = fork(); // Створюємо новий процес
    if (process < 0) {// Перевіряємо чи вдалося створити дочірній процес
        printf("Невдалось створити дочірній процес\n");
    } else if (process == 0) {// Код, що виконується дочірнім процесом
        printf("Дочірній процес. Айді процесу = %d\n", getpid());

        char *str = (char *) shmat(shmid, (void *) 0, 0);  // Приєднуємося до спільного розділу пам'яті
        // ідентифікатор розділу пам'яті

        // (void *) 0 - це позначення початку області пам'яті. У вашому випадку (void *) 0 вказує на те,
        // що функція shmat повинна вибрати будь-яке вільне місце у розділі пам'яті для приєднання.

        // 0 - це флаг, який вказує, що ми хочемо лише читати та писати в спільний розділ пам'яті.


        printf("Значення яке було введено батьківським процесом у спільну пам'ять: %s\n", str);

        strcpy(str, "Hello world");    // Записуємо дані в спільний розділ пам'яті

        shmdt(str); // Від'єднуємося від спільного розділу пам'яті

        exit(1);
    } else { // Код, що виконується батьківським процесом
        printf("Батьківський процес. Айді процесу = %d\n", getpid());
        int status;

        char *str = (char *) shmat(shmid, (void *) 0, 0);
        strcpy(str, "тестовий рядок"); // Записуємо дані в спільний розділ пам'яті

        wait(&status);

        printf("Значення яке було введено дочірнім процесом у спільну пам'ять: %s\n",
               str); // Читаємо дані з спільного розділу пам'яті

        if (WIFEXITED(status))
            printf("Дочірній процес завершився зі статусом %d\n", WEXITSTATUS(status));
        else
            printf("Дочірній процес завершився некоректно\n");

        shmdt(str); // Від'єднуємося від спільного розділу пам'яті
    }

    shmctl(shmid, IPC_RMID, NULL); // Видаляємо спільний розділ пам'яті
    // shmid - ідентифікатор розділу пам'яті
    // IPC_RMID - цей флаг вказує, що розділ пам'яті повинен бути видалений
    // NULL - це покажчик на структуру, яка містить додаткові параметри для спільного розділу пам'яті.
    // Ми передаємо NULL, оскільки ми не хочемо передавати додаткові параметри.

    return 0;
}
