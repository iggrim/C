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
    hints.ai_socktype = SOCK_STREAM; // потоковый сокет TCP // для UDP - SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // AI_PASSIVE указывает getaddrinfo() назначить   сокету адрес текущего хоста. Не нужно указывать свой адрес вручную. Иначе можно указать свой IP в качестве первого параметра getaddrinfo(),(у нас 0 или NULL.)

    struct addrinfo *bind_address; // ----обратить внимание---, bind_address получит указатель на указатель, согласно прототипа функции getaddrinfo
    // getaddrinfo(0, "8080", &hints, &bind_address);
    getaddrinfo(0, "5000", &hints, &bind_address); // Мы используем getaddrinfo() для заполнения структуры struct addrinfo // возвращает указатель на указатель на связанный список результатов bind_address // bind_address - указатель на на указатель
	/*
	Прототип getaddrinfo
	int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res); // res - указательна на указатель
	node - это имя или IP адрес хоста, к которому надо подключиться, т.е. у нас это адрес на котором будем ждать соединений // если 0 - any available network interface,
	service - порт, на котором будем ждать соединений
	
	getaddrinfo(..) устанавливает значение res для указания на выделяемый динамически связанный список структур addrinfo, связанных компонентом ai_next. Существует несколько причин того, почему связанный список может содержать более одной структуры addrinfo, включая: сетевой хост, имеющий несколько адресов; службу, доступную несколькии сокетным протоколам (например, один ее адрес - это SOCK_STREAM, а второй адрес - SOCK_DGRAM).
	*/


    printf("Creating socket...\n");
	// Creating socket
    SOCKET socket_listen; // SOCKET - это typedef для беззнакового int в заголовках Winsock
//1
    socket_listen = socket(bind_address->ai_family,
            bind_address->ai_socktype, 
			bind_address->ai_protocol); // ai_protocol - может быть установлен в 0 для выбора правильного протокола для заданного типа: tcp или udp
    if (!ISVALIDSOCKET(socket_listen)) {
		// Стандартный вывод ошибок stderr, на экран
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());  //  макрос вызывает WSAGetLastError()
        return 1;
    }


    printf("Binding socket to local address...\n");
	// связать сокет с адресом и портом, полученным из getaddrinfo()
	// int bind(int sockfd, struct sockaddr *bind_address, int addrlen); 
	// bind_address это структура типа addrinfo, а bind_address->ai_addr - это структура sockaddr
	// struct sockaddr содержит  информацию об адресе - порт и IP address	
//2 		
	if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(bind_address); // освобождаем память

//3
    printf("Listening...\n");
    if (listen(socket_listen, 10) < 0) {  // В случае успеха возвращается ноль. При ошибке возвращается -1, а errno устанавливается должным образом.   // 10 соединений можно поставить в очередь
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }


    printf("Waiting for connection...\n");
    struct sockaddr_storage client_address;  // В структуре SOCKADDR_STORAGE хранится информация об адресе сокета IPv4 или IPv6 . Будет приводится к типу struct sockaddr*
    socklen_t client_len = sizeof(client_address);
	
	// Когда новое соединение будет установлено accept создаст для него новый сокет.  
	// Исходный сокет продолжает прослушивать новые соединения, но новый сокет, возвращаемый функцией accept(), может использоваться для отправки и получения данных через вновь установленное соединение
//4    
	SOCKET socket_client = accept(socket_listen,
            (struct sockaddr*) &client_address, &client_len);  // параметр client_address это структура типа sockaddr_storage содержит адрес IPv4 или IPv6 клиента, приводится к типу struct sockaddr 
			// sockaddr_storage, созданна достаточно большой, чтобы содержать обе IPv4 и IPv6 структуры. Судя по некоторым вызовам вы не знаете наперёд каким адресом  загружать вашу структуру  sockaddr: IPv4 или IPv6.Так передайте эту параллельную структуру, подобную struct sockaddr, только больше, и приведите к нужному типу
			// исходный сокет продолжает прослушивать новые соединения, но новый сокет, возвращаемый функцией accept(), может использоваться для отправки и получения данных через вновь установленное соединение.	
			// Структура SOCKADDR используется для хранения IP-адреса компьютера, участвующего в обмене данными через сокеты Windows
			
    if (!ISVALIDSOCKET(socket_client)) {
		// Стандартный вывод ошибок stderr, на экран
        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());  // Стандартный вывод ошибок stderr, на экран
        return 1;
    }


    printf("Client is connected... ");
    char address_buffer[100];  // рекомендуется где-то регистрировать сетевые подключения
    // int getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags);
	getnameinfo((struct sockaddr*)&client_address,
            client_len, 
			address_buffer, // имя узла 
			sizeof(address_buffer), 
			0, // имя сервиса (http, ftp и т.д.)
			0,
            NI_NUMERICHOST); // NI_NUMERICHOST. Если этот флаг установлен, то имя машины возвращается в числовой форме(напр. 127.0.0.1). (Если этот флаг не установлен, в этом случае имя узла не будет найдено.) 
    printf("%s\n", address_buffer);


    printf("Reading request...\n");
    char request[1024];
//5	
    int bytes_received = recv(socket_client, request, 1024, 0);
    printf("Received %d bytes.\n", bytes_received);
    //printf("%.*s", bytes_received, request); // строка формата "%.*s" сообщает функции printf, то что мы хотим напечатать определенное количество символов - bytes_receive из того, что получили в ответ в массив request


    printf("Sending response...\n");
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/plain\r\n\r\n" // Заголовок ответа HTTP заканчивается пустой строкой
        "Local time is: ";
//6		
    int bytes_sent = send(socket_client, response, strlen(response), 0); // ответ для браузера ( стандартные заголовоки HTTP-ответа) // Если сокет закрыт, send() возвращает -1. 
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

    time_t timer; // В одной системе time_t может быть unsigned long, а в другой — unsigned long long.
    time(&timer); //Функция time() возвращает текущее календарное время системы. // Функция time() может вызываться либо с указателем NULL, либо с указателем на переменную time_t. В последнем случае этот аргумент также получает значение календарного времени. 
//6
    char *time_msg = ctime(&timer); //  преобразование времени в текстовую строку
    bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0); // Если сокет закрыт, send() возвращает -1. 
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(time_msg));

    printf("Closing connection...\n");
    CLOSESOCKET(socket_client); 
	// #define CLOSESOCKET(s) closesocket(s) // win
	// #define CLOSESOCKET(s) close(s)		// unix
	
    printf("Closing listening socket...\n");
    CLOSESOCKET(socket_listen);


#if defined(_WIN32)
    WSACleanup();
#endif


    printf("Finished.\n");

    return 0;
}

/*
При установке или получении параметров сокета на IPPROTO_IP уровне приложения C / C ++ Winsock, предназначенного для Windows, включение правильного заголовка и файла библиотеки в проект программы имеет решающее значение. Если файлы заголовка и библиотеки не совпадают должным образом setsockopt или getsockopt могут завершиться ошибкой выполнения 10042 ( WSAENOPROTOOPT). В некоторых случаях, даже если API возвращается успешно, значение параметра, которое вы устанавливаете или получаете, может не соответствовать вашим ожиданиям.

Чтобы избежать этой проблемы, соблюдайте следующие правила:

    Программа, которая включает, Winsock.hдолжна связываться только с Wsock32.lib.
    Программа , которая включает в себя Ws2tcpip.hможет связать либо с Ws2_32.lib или Wsock32.lib.

    Ws2tcpip.h должен быть явно включен после Winsock2.h, чтобы использовать параметры сокета на этом уровне.

*/

/*
struct addrinfo { 
  int              ai_flags;      // AI_PASSIVE, AI_CANONNAME,  т.д. 
  int              ai_family;     // AF_INET, AF_INET6, AF_UNSPEC 
  int              ai_socktype;   // SOCK_STREAM, SOCK_DGRAM 
  int              ai_protocol;   // используйте 0 для"any" 
  size_t           ai_addrlen;    // размер ai_addr в байтах  
  struct sockaddr  *ai_addr;      // struct sockaddr_in или _in6 
  char             *ai_canonname; // полное каноническое имя хоста 
  struct addrinfo  *ai_next;      // связанный список, следующий 
 }; 
*/

/*
  struct in_addr { 
  uint32_t  s_addr;  // Интернет адрес  // это 32-битный int (4 байта) 
};
*/

/*
В структуре SOCKADDR_STORAGE хранится информация об адресе сокета.
typedef struct sockaddr_storage {
  short   ss_family;  // Семейство ----адресов---- сокета, например AF_INET.
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
// sa_data  содержит ----адрес назначения и номер порта---- для сокета.
*/

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
