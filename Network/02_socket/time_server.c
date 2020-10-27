#if defined(_WIN32) // Проверить тип ОС с помощью директивы препроцессора
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601 // Windows 7  
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // игнорируется gcc

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#endif


#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET) // макрос для винды // function socket() returns INVALID_SOCKET if it fails
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#else
#define ISVALIDSOCKET(s) ((s) >= 0) // макрос для юникс // Unix, socket() returns a negative number on failure
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)
#endif


#include <stdio.h>
#include <string.h>
#include <time.h>  // для функции time() 


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
	
	// заполнить адресные структуры с помощью getaddrinfo()
	// hints – набор указаний для getaddrinfo() 
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_STREAM; // потоковый сокет TCP
    hints.ai_flags = AI_PASSIVE; // AI_PASSIVE указывает getaddrinfo() назначить   сокету адрес текущего хоста. Не нужно указывать свой адрес вручную. Иначе можно указать свой IP в качестве первого параметра getaddrinfo(),(у нас 0 или NULL.)

    struct addrinfo *bind_address;
    // getaddrinfo(0, "8080", &hints, &bind_address);
    getaddrinfo(0, "5000", &hints, &bind_address); // Мы используем getaddrinfo() для заполнения структуры struct addrinfo // возвращает указатель на указатель на связанный список результатов bind_address
	/*
	int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res); 
	node - это имя или IP адрес хоста, к которому надо подключиться, т.е. адрес на котором будем ждать соединений // если 0 - any available network interface,
	service - порт, на котором будем ждать соединений
	*/


    printf("Creating socket...\n");
	// Creating socket
    SOCKET socket_listen; // SOCKET - это typedef для беззнакового int в заголовках Winsock
    socket_listen = socket(bind_address->ai_family,
            bind_address->ai_socktype, bind_address->ai_protocol); // ai_protocol - может быть установлен в 0 для выбора правильного протокола для заданного типа: tcp или udp
    if (!ISVALIDSOCKET(socket_listen)) {
		// Стандартный вывод ошибок stderr, на экран
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());  //  макрос вызывает WSAGetLastError()
        return 1;
    }


    printf("Binding socket to local address...\n");
	// связать сокет с адресом и портом, полученным из getaddrinfo()
	// int bind(int sockfd, struct sockaddr *my_addr, int addrlen); 
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(bind_address); // освобождаем память


    printf("Listening...\n");
    if (listen(socket_listen, 10) < 0) {  // 10 оединений можно поставить в очередь
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }


    printf("Waiting for connection...\n");
    struct sockaddr_storage client_address;  // В структуре SOCKADDR_STORAGE хранится информация об адресе сокета.
    socklen_t client_len = sizeof(client_address);
	
	// Когда новое соединение будет установлено accept создаст для него новый сокет.  
	// Исходный сокет продолжает прослушивать новые соединения, но новый сокет, возвращаемый функцией accept(), может использоваться для отправки и получения данных через вновь установленное соединение
    SOCKET socket_client = accept(socket_listen,
            (struct sockaddr*) &client_address, &client_len);  // Структура SOCKADDR используется для хранения IP-адреса компьютера, участвующего в обмене данными через сокеты Windows
    if (!ISVALIDSOCKET(socket_client)) {
		// Стандартный вывод ошибок stderr, на экран
        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());  // Стандартный вывод ошибок stderr, на экран
        return 1;
    }


    printf("Client is connected... ");
    char address_buffer[100];  // рекомендуется где-то регистрировать сетевые подключения
    getnameinfo((struct sockaddr*)&client_address,
            client_len, address_buffer, sizeof(address_buffer), 0, 0,
            NI_NUMERICHOST);
    printf("%s\n", address_buffer);


    printf("Reading request...\n");
    char request[1024];
    int bytes_received = recv(socket_client, request, 1024, 0);
    printf("Received %d bytes.\n", bytes_received);
    //printf("%.*s", bytes_received, request);


    printf("Sending response...\n");
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/plain\r\n\r\n" // Заголовок ответа HTTP заканчивается пустой строкой
        "Local time is: ";
		
    int bytes_sent = send(socket_client, response, strlen(response), 0); // ответ для браузера ( стандартные заголовоки HTTP-ответа)
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

    time_t timer; // В одной системе time_t может быть unsigned long, а в другой — unsigned long long.
    time(&timer); //Функция time() возвращает текущее календарное время системы. // Функция time() может вызываться либо с указателем NULL, либо с указателем на переменную time_t. В последнем случае этот аргумент также получает значение календарного времени. 
    char *time_msg = ctime(&timer); //  преобразование времени в текстовую строку
    bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(time_msg));

    printf("Closing connection...\n");
    CLOSESOCKET(socket_client); // #define CLOSESOCKET(s) closesocket(s)

    printf("Closing listening socket...\n");
    CLOSESOCKET(socket_listen);


#if defined(_WIN32)
    WSACleanup();
#endif


    printf("Finished.\n");

    return 0;
}

/*
typedef struct addrinfo {
  int             ai_flags;
  int             ai_family;
  int             ai_socktype;
  int             ai_protocol;
  size_t        ai_addrlen;
  char         *ai_canonname;
  struct sockaddr *ai_addr;
  struct addrinfo *ai_next;
} ADDRINFOA, *PADDRINFOA;
*/

/*
В структуре SOCKADDR_STORAGE хранится информация об адресе сокета.
typedef struct sockaddr_storage {
  short   ss_family;
  char    __ss_pad1[_SS_PAD1SIZE];
  __int64 __ss_align;
  char    __ss_pad2[_SS_PAD2SIZE];
} SOCKADDR_STORAGE, *PSOCKADDR_STORAGE;
*/

/*
Структура SOCKADDR используется для хранения IP-адреса компьютера, участвующего в обмене данными через сокеты Windows.
struct sockaddr {
    unsigned short sa_family;
    char           sa_data[14];
};
*/
