#include "chap03.h"
#include <ctype.h>

int main() {

#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif


    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);


    printf("Creating socket...\n");
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family,
            bind_address->ai_socktype, bind_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_listen)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }


    printf("Binding socket to local address...\n");
    if (bind(socket_listen,
                bind_address->ai_addr, bind_address->ai_addrlen)) {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(bind_address);


    printf("Listening...\n");
    if (listen(socket_listen, 10) < 0) {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    fd_set master;
	/*
	typedef struct fd_set {
	unsigned int count;
	int fd[FD_SETSIZE];
	} fd_set;
	*/
	
    FD_ZERO(&master);	
    FD_SET(socket_listen, &master); // добавляем socket_listen в master
	/* Макросы
	FD_SET(int fd, fd_set *set);     Добавляет  fd  в set. 
	FD_CLR(int fd, fd_set *set);     Удаляет  fd  из  set. 
	FD_ISSET(int fd, fd_set *set);   Возвращает  true  если  fd  есть в set. 
	FD_ZERO(fd_set *set);     Очищает  set
	*/
	
    SOCKET max_socket = socket_listen;

    printf("Waiting for connections...\n");


    while(1) {
        fd_set reads;
        reads = master;	// каждый раз в цикле делаем копию с чистой структуры masterт.к. вызов select() изменяет содержимое наборов в соответствии с правилами
		
		// int	select(int nfds, fd_set	*readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
		// select проверяет, готовы ли какие-либо сокеты для чтения (readfds), записи (writefds) или имеют ли исключительное состояние pending (exceptfds).
		// Если выбор завершился успешно, он возвращает количество готовых дескрипторов сокета. select возвращает 0, если срок истекает до того, как будут выбраны какие-либо сокеты. В случае ошибки select возвращает -1.
		// select() требует, чтобы мы передавали число, превышающее самый большой дескриптор сокета, который мы собираемся отслеживать
        if (select(max_socket+1, &reads, 0, 0, 0) < 0) {
            fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }
		// select не сразу возвращает управление. Она возвращает управление в четырех случаях:  ---хотя бы на одном сокете произошли изменения; ---пришел заданный тайм-аут;  ---пришел сигнал--- ( может такое быть, что и тайм-аут не истек, и сокеты не изменились, а select завершилась), напр. типичный сигнал - Ctrl+C.

        SOCKET i;
        for(i = 1; i <= max_socket; ++i) {
			// с помощью FD_ISSET можно проверить, были ли изменения по данному сокету (проверять нужно в каждом массиве
            if (FD_ISSET(i, &reads)) { 

                if (i == socket_listen) {
                    struct sockaddr_storage client_address;
                    socklen_t client_len = sizeof(client_address);
					
					// новый сокет по запросу от клиента
					// сиситема сама разруливает номера файловых дескрипторов ссылающихся на сокет
                    SOCKET socket_client = accept(socket_listen,
                            (struct sockaddr*) &client_address,
                            &client_len);
							
                    if (!ISVALIDSOCKET(socket_client)) {
                        fprintf(stderr, "accept() failed. (%d)\n",
                                GETSOCKETERRNO());
                        return 1;
                    }
					
					// заносим новый сокет в структуру master(не в копию)
                    FD_SET(socket_client, &master); 
                    if (socket_client > max_socket)
                        max_socket = socket_client; // и увеличиваем max_socket

                    char address_buffer[100];
                    getnameinfo((struct sockaddr*)&client_address,
                            client_len,
                            address_buffer, sizeof(address_buffer), 0, 0,
                            NI_NUMERICHOST);
                    printf("New connection from %s\n", address_buffer);

                } else {
                    char read[1024];
                    int bytes_received = recv(i, read, 1024, 0);
					
                    if (bytes_received < 1) { // Если клиент отключился
                        FD_CLR(i, &master); // удаляем сокет из основной структуры master
                        CLOSESOCKET(i); // закрываем сокет
                        continue;
                    }
					
					// если получили данные, рассылаем на подключенные сокеты кроме основного и передающего
					// например подключено 4 сокета - 1 основной после функции listen и 3 после функции accept(т.е. подключилось три клиента) и они поочередно переданы в структуру master(тип fd_set) через макрос FD_SET, в цикле while функция select мониторит структуру reads(копия структуры master) и мы постоянно пробегаемся по этой --копии-- в цикле while-for и пусть например сработал третий сокет в select, т.е. изменилась структура reads(в циклах while-for), тогда мы исключаем этот сокет(который для reads(в циклах while-for) равен i=3, а для masters(в циклах while-for- else равен j=3) из сокетов для рассылки сообщения, также как и сновной сокет равный socket_listen
                    SOCKET j;
                    for (j = 1; j <= max_socket; ++j) {
                        if (FD_ISSET(j, &master)) { // как бы дополнительная проверка???
                            if (j == socket_listen || j == i) // исключаем этот сокет из сокетов для рассылки сообщения, также как и сновной сокет
                                continue;
                            else
                                send(j, read, bytes_received, 0); // Если сокет закрыт, send() возвращает -1. 
                        }
                    }
                }
            } //if FD_ISSET
        } //for i to max_socket
    } //while(1)



    printf("Closing listening socket...\n");
    CLOSESOCKET(socket_listen);

#if defined(_WIN32)
    WSACleanup();
#endif


    printf("Finished.\n");

    return 0;
}

/*
int select(int nfds, fd_set * readfds, fd_set * writefds,  fd_set * exceptfds, const struct timeval * timeout);

select проверяет, готовы ли какие-либо сокеты для чтения ( readfds ), записи ( writefds ) или имеют ли исключительное состояние pending ( exceptfds ).

timeout указывает максимальное время для завершения выбора. 

nfds указывает максимальное количество сокетов для проверки. Если тайм-аут является нулевым указателем, выбирать блоки бесконечно. 

timeout указывает на временную структуру. Значение тайм-аута 0 вызывает немедленный возврат select. Такое поведение полезно в приложениях, которые периодически опрашивают свои сокеты. Прибытие out-of-band data - единственное возможное исключительное условие. 

fd_set - это тип, определенный в заголовочном файле <sys / types.h>. fd_set определяет набор файловых дескрипторов, над которыми работает select. Передача NULL для любого из трех аргументов fd_set указывает, что select не должен отслеживать это условие. По возвращении каждый из входных наборов модифицируется, чтобы указать подмножество готовых описаний. Их можно найти с помощью макроса FD_ISSET.

Если выбор завершился успешно, он возвращает количество готовых дескрипторов сокета. select возвращает 0, если срок истекает до того, как будут выбраны какие-либо сокеты. В случае ошибки select возвращает -1.
*/
