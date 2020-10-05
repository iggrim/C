// Wiki, пример использования потоков

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

void wait_thread (void);
void* thread_func (void*);

int main (int argc, char *argv[], char *envp[]) {
    pthread_t thread;
    if (pthread_create(&thread,NULL,thread_func,NULL)) return EXIT_FAILURE;
    for (unsigned int i = 0; i < 20; i++) {
        puts("a");
        wait_thread();
    }
    if (pthread_join(thread,NULL)) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

void wait_thread (void) {
    time_t start_time = time(NULL);
    while(time(NULL) == start_time) {}
}

void* thread_func (void* vptr_args) {
    for (unsigned int i = 0; i < 20; i++) {
        fputs("b\n",stderr);
        wait_thread();
    }
    return NULL;
}
/*
Основные функции стандарта

Pthreads определяет набор типов и функций на языке программирования Си. Заголовочный файл — pthread.h.

    Типы данных:
        pthread_t: дескриптор потока;
        pthread_attr_t: перечень атрибутов потока;
        pthread_barrier_t: барьер;
        pthread_barrierattr_t: атрибуты барьера;
        pthread_cond_t: условная переменная;
        pthread_condattr_t: атрибуты условной переменной;
        pthread_key_t: данные, специфичные для потока;
        pthread_mutex_t: мьютекс;
        pthread_mutexattr_t: атрибуты мьютекса;
        pthread_rwlock_t: мьютекс с возможностью эксклюзивной блокировки;
        pthread_rwlockattr_t: атрибуты этого мьютекса;
        pthread_spinlock_t: спинлок;
    Функции управления потоками:
        pthread_create(): создание потока.
        pthread_exit(): завершение потока (должна вызываться функцией потока при завершении).
        pthread_cancel(): отмена потока.
        pthread_join(): подключиться к другому потоку и ожидать его завершения; поток, к которому необходимо подключиться, должен быть создан с возможностью подключения (PTHREAD_CREATE_JOINABLE).
        pthread_detach(): отключиться от потока, сделав его при этом отдельным (PTHREAD_CREATE_DETACHED).
        pthread_attr_init(): инициализировать структуру атрибутов потока.
        pthread_attr_setdetachstate(): указывает параметр "отделимости" потока (detach state), который говорит о возможности подключения к нему (при помощи pthread_join) других потоков (значение PTHREAD_CREATE_JOINABLE) для ожидания окончания или о запрете подключения (значение PTHREAD_CREATE_DETACHED); ресурсы отдельного потока (PTHREAD_CREATE_DETACHED) при завершении автоматически освобождаются и возвращаются системе.
        pthread_attr_destroy(): освободить память от структуры атрибутов потока (уничтожить дескриптор).
    Функции синхронизации потоков:
        pthread_mutex_init(), pthread_mutex_destroy(), pthread_mutex_lock(), pthread_mutex_trylock(), pthread_mutex_unlock(): с помощью мьютексов.
        pthread_cond_init(), pthread_cond_signal(), pthread_cond_wait(): с помощью условных переменных.
*/

