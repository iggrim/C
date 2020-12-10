#include "chap03.h"

#if defined(_WIN32)
#include <conio.h>
#endif

int main(int argc, char *argv[]) {

#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif

    if (argc < 3) {
        fprintf(stderr, "usage: tcp_client hostname port\n");
        return 1;
    }

    printf("Configuring remote address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *peer_address;
    if (getaddrinfo(argv[1], argv[2], &hints, &peer_address)) {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }


    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
	// getnameinfo convert the address into	a standard notation	address
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
            address_buffer, // имя узла
			sizeof(address_buffer),
            service_buffer, // имя сервиса (http, ftp и т.д.)
			sizeof(service_buffer),
            NI_NUMERICHOST); // Если этот флаг установлен, то имя машины возвращается в числовой форме(напр. 127.0.0.1). (Если этот флаг не установлен, в этом случае имя узла не будет найдено.) 
    printf("%s %s\n", address_buffer, service_buffer);


    printf("Creating socket...\n");
	// SOCKET - это typedef для беззнакового int в заголовках Winsock. В спецификации WinSock тип SOCKET определен как беззнаковый (unsigned)
    SOCKET socket_peer;
    socket_peer = socket(peer_address->ai_family,
            peer_address->ai_socktype, peer_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_peer)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }


    printf("Connecting...\n");
    if (connect(socket_peer,
                peer_address->ai_addr, peer_address->ai_addrlen)) {
        fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(peer_address);

    printf("Connected.\n");
    printf("To send data, enter text followed by enter.\n");

    while(1) {
		/*
		typedef struct fd_set {
		unsigned int count;
		int fd[FD_SETSIZE];
		} fd_set;
		*/
		/* Макросы
		FD_SET(int fd, fd_set *set);     Добавляет  fd  в set. 
		FD_CLR(int fd, fd_set *set);     Удаляет  fd  из  set. 
		FD_ISSET(int fd, fd_set *set);   Возвращает  true  если  fd  есть в set. 
		FD_ZERO(fd_set *set);            Очищает  set
		*/
        fd_set reads;
        FD_ZERO(&reads); // обязательно обнулить структуру перед её заполнением
        FD_SET(socket_peer, &reads); // устанавливаем файловый дескриптор указывающий на сокет
#if !defined(_WIN32)
        FD_SET(0, &reads); 
#endif

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000; // микросекунды

		// int select (int nfds, fd_set * readfds, fd_set * writefds,  fd_set * exceptfds, const struct timeval * timeout); // nfds указывает максимальное количество сокетов для проверки
		// select проверяет, готовы ли какие-либо сокеты для чтения, записи или имеют ли исключительное состояние pending.
        // у нас select в цикле следит за изменением второго параметра reads (тип - структура fd_set) отвечающего за готовность сокета для чтения 
		// блокирeт поток пока любой из сокетов в наборе не будет готов для чтения или по истечении timeout
		// в системах на основе Unix select() также можно использовать не только на сокетах, но и для ---файлового и терминального ввода-вывода,--- 
		// Если выбор завершился успешно, она возвращает количество готовых дескрипторов сокета. select возвращает 0, если срок истекает до того, как будут выбраны какие-либо сокеты. В случае ошибки select возвращает -1.
		if (select(socket_peer+1, &reads, 0, 0, &timeout) < 0) {
            fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }

		// постоянно проверяем в цикле while параметр reads(тип - структура fd_set) 
		// в Windows reads будет установлен если получили данные от сервера,
		// в Unix - если получили данные от сервера или от stdin
		// с помощью FD_ISSET можно проверить, были ли ---изменения--- по данному сокету
        if (FD_ISSET(socket_peer, &reads)) { 
            char read[4096];
            int bytes_received = recv(socket_peer, read, 4096, 0);
            if (bytes_received < 1) {
                printf("Connection closed by peer.\n");
                break;
            }
            printf("Received (%d bytes): %.*s", bytes_received, bytes_received, read); // // строка формата "%.*s" сообщает функции printf, то что мы хотим напечатать определенное количество символов - bytes_receive из того, что получили в ответ в массив read        }

#if defined(_WIN32)
        if(_kbhit()) {
#else
        if(FD_ISSET(0, &reads)) { // в Unix, 0 - дескриптор файла для stdin
#endif
            char read[4096];
            if (!fgets(read, 4096, stdin)) break;
            printf("Sending: %s", read);
            int bytes_sent = send(socket_peer, read, strlen(read), 0); // Если сокет закрыт, send() возвращает -1. 
            printf("Sent %d bytes.\n", bytes_sent);
        }
    } //end while(1)

    printf("Closing socket...\n");
    CLOSESOCKET(socket_peer);

#if defined(_WIN32)
    WSACleanup();
#endif

    printf("Finished.\n");
    return 0;
}

/*
int getnameinfo(const struct sockaddr *sa, socklen_t salen,
                char *host, size_t hostlen,
                char *serv, size_t servlen, 
				int flags);
Функция getnameinfo() предназначена для перевода сетевого адреса в имя машины, способом, который не зависит от сетевого протокола. Она сочетает в себе действия функций gethostbyaddr() и getservbyport(), и является функцией обратной функции getaddrinfo(). Аргумент sa - это указатель на структуру адреса сокета (типа sockaddr_in или sockaddr_in6) размером salen, которая содержит IP-адрес и номер порта. Аргументы host и serv указывают на буферы (размером hostlen и servlen соответственно), которые будут содержать возвращаемые значения.
Если при вызове не требуется определять имя машины (или имя сервиса), тогда аргумент host (или serv) должен указывает на NULL или параметр hostlen (или servlen) должен быть равен нулю. Однако, по крайней мере один параметр, имя машины или имя сервиса, должно быть запрошено. 
При успешном завершении возвращается 0, имя узла и имя сервиса. Возможно сокращение имен в буфере до указанной длины. При ошибке возвращается ненулевое значение, а глобальная переменная errno устанавливается в соответствующее значение.  

Флаги. NI_NUMERICHOST. 
Если этот флаг установлен, то имя машины возвращается в числовой форме(напр. 127.0.0.1). (Если этот флаг не установлен, в этом случае имя узла не будет найдено.) 
*/
