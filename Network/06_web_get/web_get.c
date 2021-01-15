#include "chap06.h"

#define TIMEOUT 5.0

void parse_url(char *url, char **hostname, char **port, char** path) {
    printf("URL: %s\n", url);

    char *p;
    p = strstr(url, "://");
	/*
	strstr – поиск первого вхождения строки А в строку В.
	*/

    char *protocol = 0;
    if (p) {
        protocol = url;
        *p = 0;
        p += 3; // смещаем указатель за "://"
    } else {
        p = url;
    }

    if (protocol) {
        if (strcmp(protocol, "http")) {
            fprintf(stderr,
                    "Unknown protocol '%s'. Only 'http' is supported.\n",
                    protocol);
            exit(1);
        }
    }

    // не парсится имя хоста,
    // напр. exeample.com:80

	//*hostname = p;
    // из указателя(**hostname) извлекаем адрес на указатель(*hostname)
    // и присваиваем ему адрес 'p'

    //while (*p && *p != ':' // ??? интересный прием, я бы сделал просто *p != ':'
    //		&& *p != '/'
    //		&& *p != '#')
    //		++p; // инкриментируем значение указателя

    // ----------сделал так:
    char * a;
    char *start = p;
    int i = 0, n = 1;

    while (*p && *p != ':' && *p != '/' && *p != '#'){
    	n++;
    	p++;
    }

    a = (char*)malloc(n * sizeof(char));
    for(i=0; i<n-1; i++){
    	a[i] = *start;
    	start++;
    }

    *hostname = a; // адрес начала имени хоста

	// ---------------------

    *port = "80";
    if (*p == ':') {
        //*p++ = 0;  //было  // инкриментируем значение указателя
        p++; // исправил
    	*port = p;  // адрес начала имени порта
    }

    while (*p && *p != '/'
    		&& *p != '#')
    		++p; // снова продвигаем указатель

    *path = p;
    if (*p == '/') {
        *path = p + 1;
    }
    //*p = 0; было


    while (*p && *p != '#')
    	++p;

    if (*p == '#') *p = 0;

    printf("host: %s\n", *hostname); // *hostname - адрес начала имени хоста
    printf("port: %s\n", *port); // *port - адрес начала имени порта
    printf("path: %s\n", *path);
}

//----------------------------

void send_request(SOCKET s, char *hostname, char *port, char *path) {
    char buffer[2048];

    sprintf(buffer, "GET /%s HTTP/1.1\r\n", path);
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", hostname, port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    /*постоянные соединения, заголовок в запросе:  Connection: Keep-Alive
      Connection: Close, закрыть TCP-соединение после получения HTTP-ответа. */
    sprintf(buffer + strlen(buffer), "User-Agent: honpwc web_get 1.0\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");
	// sprintf // похоже на printf, за исключением того, что вывод сохраняется в массиве 
    
	send(s, buffer, strlen(buffer), 0);
    printf("Sent Headers:\n%s", buffer);

	
}

//-----------------------------

SOCKET connect_to_host(char *hostname, char *port) {
    printf("Configuring remote address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *peer_address;
    if (getaddrinfo(hostname, port, &hints, &peer_address)) {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }

    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
	// peer_address->ai_addr - это структура sockaddr, одно из полей в структуре addrinfo
	// struct sockaddr содержит информацию об адресе - порт и IP address
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
            address_buffer, sizeof(address_buffer),
            service_buffer, sizeof(service_buffer),
            NI_NUMERICHOST);
    printf("%s %s\n", address_buffer, service_buffer);

    printf("Creating socket...\n");
    SOCKET server;
    server = socket(peer_address->ai_family,
            peer_address->ai_socktype, peer_address->ai_protocol);
    if (!ISVALIDSOCKET(server)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }

    printf("Connecting...\n");
	// int connect(int sockfd, struct sockaddr *serv_addr, int addrlen); // справка в конце кода
    if (connect(server,
                peer_address->ai_addr, peer_address->ai_addrlen)) {
        fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }
    freeaddrinfo(peer_address);

    printf("Connected.\n\n");

    return server;
}

//----------------------------


int main(int argc, char *argv[]) {

#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif


    //if (argc < 2) {
    //    fprintf(stderr, "usage: web_get url\n");
    //    return 1;
    //}
    //char *url = argv[1];
	
	// ------для отладки
    // char *url = "exeample.com:80";
	char *url = "ya.ru:443";
	// -----------------

    char *hostname, *port, *path;

    parse_url(url, &hostname, &port, &path);
    // здесь hostname, port, path еще не инициализированные указатели
    // hostname, port, path это указатели на указатели

    SOCKET server = connect_to_host(hostname, port);

    // здесь hostname, port, после парсинга(parse_url(...) ) уже проинициализированы
    send_request(server, hostname, port, path);

    const clock_t start_time = clock();

// #define RESPONSE_SIZE 32768 // 
#define RESPONSE_SIZE 131072
    char response[RESPONSE_SIZE+1]; // массив под полученные данные
    char *p = response, *q; // 

    // адрес end больше чем адрес response на 131072
    char *end = response + RESPONSE_SIZE;
	// Указатель body используется для запоминания начала тела 
	// ответа HTTP после получения
    char *body = 0; 

    // создаем , просто набор констант без имени перечисления(enum)
    enum {length, chunked, connection}; // 0, 1, 2 
    
    int encoding = 0;
    int remaining = 0;

    while(1) {

        if ((clock() - start_time) / CLOCKS_PER_SEC > TIMEOUT) {
            fprintf(stderr, "timeout after %.2f seconds\n", TIMEOUT);
            return 1;
        }

        if (p == end) { // первоначально *end-*p = RESPONSE_SIZE = 131072
        	fprintf(stderr, "out of buffer space\n");
            return 1;
        }

        fd_set reads;
        FD_ZERO(&reads);
        FD_SET(server, &reads);

        // Структура struct timeval представляет календарное время. 
        struct timeval timeout;

        timeout.tv_sec = 0;
        timeout.tv_usec = 200000;

        if (select(server+1, &reads, 0, 0, &timeout) < 0) {
            fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }

        if (FD_ISSET(server, &reads)) {

        	// int recv(int s, char * buf, int len, int flags);
        	// Первый аргумент - сокет-дескриптор, из которого читаются данные.
        	// Второй и третий аргументы - соответственно, адрес и длина буфера
        	// для записи читаемых данных. Четвертый параметр - это комбинация
        	// битовых флагов, управляющих режимами чтения
            int bytes_received = recv(server, p, end - p, 0);
			// p это - char *p = response[RESPONSE_SIZE+1] 			
            // длинна буфера вычисляется(end - p) по ходу выполнения функции

            if (bytes_received < 1) {
				// enum {length, chunked, connection}; // 0, 1, 2
				// при попытке прочитать новые данные мы можем обнаружить, 
				// что сокет был закрыт веб-сервером. Если это так, мы проверяем, 
				// ожидали ли мы, что закрытое соединение укажет на конец передачи. 
				// Это так, если encoding == connection
				// Указатель body используется для запоминания начала тела 
				// ответа HTTP после получения
                if (encoding ==   // в цикле while, encoding = length; 
								connection && body) { // проверить body // connection просто цело число???
					// вывод в консоль body на длинну (end - body)
                    printf("%.*s", (int)(end - body), body);
                }			

                printf("\nConnection closed by peer.\n");
                break;
            }

            // printf("Received (%d bytes):    '%.*s'", bytes_received,    bytes_received, p);
            /* Выводим количество полученых байт и все, что пришло: заголовок и тело, 
			по длинне bytes_received
            */

			// продвигаем указатель на адрес следующий за полученными данными
            p += bytes_received; 
            *p = 0;

            // p продвинули, но response попрежнему указывает на начало массива с данными
            if (!body &&
                    (body = strstr(response, "\r\n\r\n"))) { // ищем вхождение "\r\n\r\n"
            	// после проверки ответа на две пустые строки присваиваем значению body ноль
          	*body = 0;
            	// и продвигаем указатель body за "\r\n\r\n"(конец строки заголовка + конец пустой строки, по 1 байтe на символ)
            	// в отладчике, продвижение на "ff9\r\n....."
                body += 4;

                // ===printf выведет response в консоль тоько до символа перевода строки===
                // Если сервер хочет начать отправку данных до того, как станет
                // известна длина тела, он не может использовать строку заголовка
                // Content-Length. В этом случае сервер может отправить строку
                // заголовка Transfer-Encoding: chunked. Эта строка заголовка
                // указывает клиенту, что тело ответа будет отправлено отдельными
                // частями. Каждый фрагмент начинается с его длины, закодированной
                // в формате base-16 (шестнадцатеричный), за которым следует новая
                // строка, а затем данные фрагмента. Все тело HTTP заканчивается
                // блоком нулевой длины.
                printf("Received Headers:\n%s\n", response);
                q = strstr(response, "\nContent-Length: ");
                if (q) {
					// encoding = 0; length  = length(из enum индекс length равен нулю)
                    encoding = length; 
					
					// Функция strchr выполняет поиск первого вхождения символа в строку. Возвращает указатель на первое вхождение символа в строке. Завершающий нулевой символ считается частью Си-строки. Таким образом, он также может быть найден для получения указателя на конец строки.
                    q = strchr(q, ' '); // ищем вхождение пробела, 
                    // продвигаем указатель на 1 байт(' ') и считываем строку до символа перевода строки
					// с помощью strtol
					q += 1; 

                    // Функция strtol преобразовывает строку в long int. Анализируя строку string,
                    // strtol интерпретирует её содержимое в целое число типа long int.
                    remaining = strtol(q, 0, 10); // представить число в десятично системе

                } else {
                    q = strstr(response, "\nTransfer-Encoding: chunked");
                    if (q) {
						// enum {length, chunked, connection}; // 0, 1, 2
                        encoding = chunked; // encoding = 1; chunked  = chunked(из enum индекс chunked равен 1)
                        remaining = 0;
                    } else {
                        encoding = connection; // enum {length, chunked, connection}; // 0, 1, 2
                    }
                }
                printf("\nReceived Body:\n");
            }

            if (body) {
                if (encoding == length) { // из enum индекс length равен нулю
                    if (p - body >= remaining) {
                        printf("%.*s", remaining, body);
                        break;
                    }
                } else if (encoding == chunked) { // из enum индекс chunked равен 1
                    do { // цель - найти следующий chunk
                        if (remaining == 0) {
                            if ((q = strstr(body, "\r\n"))) {
                                remaining = strtol(body, 0, 16); // преобразовать часть строки("ff9\r\n") в значение 16-й СС 
                                if (!remaining) goto finish; // первый проход. remaining = ff9 или 4089 (в отладчике) // второй проход - remaining = 0
                                body = q + 2; // продвигаем указатель body за \r\n ("ff9r\n")
                            } else {
                                break;
                            }
                        }
                        if (remaining && p - body >= remaining) {
                            printf("%.*s", remaining, body); // вывод body на длинну remaining
                            body += remaining + 2;
                            remaining = 0;
                        }
                    } while (!remaining);
                }
            } //if (body)
        } //if FDSET
    } //end while(1)
finish:

    printf("\nClosing socket...\n");
    CLOSESOCKET(server);

#if defined(_WIN32)
    WSACleanup();
#endif

    printf("Finished.\n");
    return 0;
}

/*

#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
Функция возвращает −1 в случае ошибки. Иначе, она возвращает целое число, 
представляющее присвоенный дескриптор.

socket() принимает три аргумента:
    domain, указывающий семейство протоколов создаваемого сокета. Этот параметр 
	задает правила использования именования и формат адреса. Например:
        PF_INET для сетевого протокола IPv4 или
        PF_INET6 для IPv6.
        PF_UNIX для локальных сокетов (используя файл).
    type (тип) один из:
        SOCK_STREAM надёжная потокоориентированная служба (TCP) (сервис) или потоковый сокет
        SOCK_DGRAM служба датаграмм (UDP) или датаграммный сокет
        SOCK_SEQPACKET надёжная служба последовательных пакетов
        SOCK_RAW Сырой сокет — сырой протокол поверх сетевого уровня.
    protocol определяет используемый транспортный протокол. Самые распространённые — 
	это IPPROTO_TCP, IPPROTO_SCTP, IPPROTO_UDP, IPPROTO_DCCP. Эти протоколы указаны 
	в <netinet/in.h>. Значение «0» может быть использовано для выбора протокола по 
	умолчанию из указанного семейства (domain) и типа (type).
*/
/*
int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);

Здесь sockfd - сокет, который будет использоваться для обмена данными с сервером,
serv_addr содержит указатель на структуру с адресом сервера, а addrlen - длину
этой структуры. Обычно сокет не требуется предварительно привязывать к локальному
адресу, так как функция connect сделает это за вас, подобрав подходящий свободный
порт. Вы можете принудительно назначить клиентскому сокету некоторый номер порта,
используя bind перед вызовом connect.
*/

/*
При установке или получении параметров сокета на IPPROTO_IP уровне приложения 
C/C++ Winsock, предназначенного для Windows, включение правильного заголовка и 
файла библиотеки в проект программы имеет решающее значение. Если файлы заголовка 
и библиотеки не совпадают должным образом setsockopt или getsockopt могут 
завершиться ошибкой выполнения 10042 ( WSAENOPROTOOPT). В некоторых случаях, 
даже если API возвращается успешно, значение параметра, которое вы
устанавливаете или получаете, может не соответствовать вашим ожиданиям.

Чтобы избежать этой проблемы, соблюдайте следующие правила:

    Программа, которая включает, Winsock.h должна связываться только с Wsock32.lib.
    Программа , которая включает в себя Ws2tcpip.h может связаться либо с Ws2_32.lib
    или Wsock32.lib.
    Ws2tcpip.h должен быть явно включен после Winsock2.h, чтобы использовать
    параметры сокета на этом уровне.
*/

/*int recv (int socket, void * buffer, size_t size, int flags)
Функция recv аналогична функции чтения, но с дополнительными флагами flags.
Возможные значения флагов описаны в разделе Параметры данных сокета.
Если для сокета установлен неблокирующий режим и данные недоступны для чтения,
recv завершается немедленно, а не ждет.

int recv(int s, char * buf, int len, int flags);
Первый аргумент - сокет-дескриптор, из которого читаются данные. Второй и третий а
ргументы - соответственно, адрес и длина буфера для записи читаемых данных. Четвертый
параметр - это комбинация битовых флагов, управляющих режимами чтения. Если аргумент
flags равен нулю, то считанные данные удаляются из сокета. Если значение flags есть
MSG_PEEK, то данные не удаляются и могут быть считаны последущим вызовом ( или
вызовами ) recv.

Функция возвращает число считанных байтов или -1 в случае ошибки. Следует отметить,
что нулевое значение не является ошибкой. Оно сигнализирует об отсутствии записанных
в сокет процессом-поставщиком данных.

Эта функция определяется как точка отмены в многопоточных программах, поэтому
нужно быть готовым к этому и убедиться, что выделенные ресурсы (например, память,
дескрипторы файлов, семафоры или что-то еще) освобождены, даже если поток отменен.
Заголовочные файлы:
sys / socket.h
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
sa_family будет либо AF_INET (IPv4) или AF_INET6
sa_data  содержит ----адрес назначения и номер порта---- для сокета.

*/

/*
	strstr – поиск первого вхождения строки А в строку В.
	#include < string.h >
	char *strstr(const char *strB, const char *strA);
	-----Аргументы:
	strB – указатель на строку, в которой ведется поиск.
	strA – указатель на искомую строку.
	-----Возвращаемое значение:
	1.NULL – если строка strA не входит в строку strB. 2.Указатель на первое вхождение
	строки strA в строку strB.
	Функция strstr ищет первое вхождение строки (за исключением признак конца строки),
	на которую указывает аргумент
	strA, в строку , на которую указывает аргумент strB. Если строка strA имеет нулевую
	длину, то функция вернет указатель на начало строки strB.
*/
