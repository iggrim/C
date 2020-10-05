// использования потоков на языке C++:

#include <cstdlib>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <pthread.h>
    
class Thread {
public:
    virtual ~Thread () {}
    virtual void run () = 0;
    int start () {
        return pthread_create( &_ThreadId, NULL,Thread::thread_func, this ); 
    }
    int wait () { 
        return pthread_join( _ThreadId, NULL ); 
    }
protected:
    pthread_t _ThreadId;
    Thread(const Thread&);
    void* thread_func(void* d) {
        (static_cast <Thread*>(d))->run(); 
        return NULL; 
    }
};

class TestingThread : public Thread {
public:
    TestingThread (const char* pcszText) : _pcszText( pcszText ) {}
    virtual void run () {
        for (unsigned int i = 0; i < 20; i++, Sleep(1000)) std::cout << _pcszText << std::endl;
    }
protected:
    const char* _pcszText;
};

int main (int argc, char *argv[], char *envp[]) {
    TestingThread ThreadA("a");
    TestingThread ThreadB("b");
    return ThreadA.start() || ThreadB.start() || ThreadA.wait() || ThreadB.wait() ? EXIT_FAILURE : EXIT_SUCCESS;
}

// ошибки ===============

/*Представленные программы используют два потока, печатающих в консоль сообщения, один, печатающий 'a', второй — 'b'. Вывод сообщений смешивается в результате переключения выполнения между потоками или одновременном выполнении на мультипроцессорных системах.

Отличие состоит в том, что программа на C создаёт один новый поток для печати 'b', а основной поток печатает 'a'. Основной поток (после печати 'aaaaa….') ждёт завершения дочернего потока.

Программа на C++ создаёт два новых потока, один печатает 'a', второй, соответственно, — 'b'. Основной поток ждёт завершения обоих дочерних потоков. */