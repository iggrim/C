#include <threads.h> // ошибка: нет файла threads.h
//#include <../include/libxml2/libxml/threads.h> // ошибка: fatal error: libxml/xmlversion.h: No such file or directory   #include <libxml/xmlversion.h>
#include <stdio.h>
#include <pthread.h>

#define SIZE 5
typedef pthread_t thrd_t;

int thread_func(void *id)
{
    /* thread local variable i. */
    static _Thread_local int i;

    /* Prints the ID passed from main() and the address of the i.
     * Running this program will print different addresses for i, showing
     * that they are all distinct objects. */
    printf("From thread:[%d], Address of i (thread local): %p\n", *(int*)id, (void*)&i);

    return 0;
}

int main(void)
{
    thrd_t id[SIZE];
    int arr[SIZE] = {1, 2, 3, 4, 5};

    /* create 5 threads. */
    for(int i = 0; i < SIZE; i++) {
        //thrd_create(&id[i], thread_func, &arr[i]);
        pthread_create(&id[i], thread_func, &arr[i]);
    }

    /* wait for threads to complete. */
    for(int i = 0; i < SIZE; i++) {
        thrd_join(id[i], NULL);
    }
}