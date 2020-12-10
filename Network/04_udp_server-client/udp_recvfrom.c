#include "chap04.h"

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
    hints.ai_socktype = SOCK_DGRAM;
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


    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    char read[1024];
    int bytes_received = recvfrom(
	        socket_listen,
            read, // полученные данные
			1024,
            0,
            (struct sockaddr*) &client_address, // адрес отправителя
			&client_len);
	/* int recv(
	        int sockfd, 
			void *buf, 
			int len, 
			int flags); */
	/*int  recvfrom(
			int sockfd, 
			void *buf, 
			int len, 
			unsigned int flags, // flags можно выставить в 0. См. доки. 
            struct sockaddr *from, 
			int *fromlen); */		

    printf("Received (%d bytes): %.*s\n",
            bytes_received, bytes_received, read);

    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(
			((struct sockaddr*)&client_address), // struct sockaddr_storage client_address
            client_len,
            address_buffer, sizeof(address_buffer),
            service_buffer, sizeof(service_buffer), // в TCP сервере эти два параметра(service_buffer) были нулевыми
            NI_NUMERICHOST | NI_NUMERICSERV);
    printf("%s %s\n", address_buffer, service_buffer);

    CLOSESOCKET(socket_listen);

#if defined(_WIN32)
    WSACleanup();
#endif

    printf("Finished.\n");
    return 0;
}

/*
В структуре SOCKADDR_STORAGE хранится информация об адресе сокета.
typedef struct sockaddr_storage {
  short   ss_family;  // Семейство ---адресов--- сокета, например AF_INET.
  char    __ss_pad1[_SS_PAD1SIZE];  // __ss_pad1 Зарезервировано. Определяется как 48-битный блокнот, который гарантирует, что SOCKADDR_STORAGE достигает 64-битного выравнивания.

  __int64 __ss_align;  // Зарезервировано. Используется компилятором для выравнивания структуры
  char    __ss_pad2[_SS_PAD2SIZE];  // Зарезервировано. Используется компилятором для выравнивания структуры
} SOCKADDR_STORAGE, *PSOCKADDR_STORAGE;

sockaddr_storage, созданна достаточно большой, чтобы содержать обе IPv4 и IPv6 
структуры. Судя по некоторым вызовам вы не знаете наперёд каким адресом  загружать вашу структуру  sockaddr: IPv4 или IPv6.Так передайте эту параллельную структуру, подобную struct sockaddr, только больше, и приведите к нужному типу.
*/

/*
Структура SOCKADDR используется для хранения IP-адреса компьютера, участвующего в обмене данными через сокеты Windows.
struct sockaddr {
    unsigned short sa_family;
    char           sa_data[14];
};
//sa_family будет либо AF_INET (IPv4) или AF_INET6 
// sa_data  содержит ---адрес назначения и номер порта--- для сокета.
*/

