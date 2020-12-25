#include "chap05.h"

// msg - указатель на начало сообщения(все что отправлено или пришло со всеми полями), p - указатель на имя, которое нужно напечатать,  end - указатель на память за концом сообщения. end требуется, чтобы мы могли проверить, не читаем ли мы после конца полученного сообщения.
const unsigned char *print_name(const unsigned char *msg, const unsigned char *p,
      const unsigned char *end) {

    if (p + 2 > end) {
        fprintf(stderr, "End of message.\n");
		exit(1);
        }

    // DNS кодирует имена особым образом. Обычно каждая метка обозначается ее длиной, за которой следует ее текст.
    // Можно повторять несколько меток, а затем имя завершается одним байтом 0.
    // Если для длины установлены два старших бита (то есть 0xc0), тогда она и следующий байт должны интерпретироваться как указатель.
    if ((*p & 0xC0) == 0xC0) { 
        const int k = ((*p & 0x3F) << 8) + p[1]; // 0x3F это 0011.1111 // значение K это 12
        p += 2;
        printf(" (pointer %d) ", k);
        print_name(msg, msg+k, end); // end для проверки границы DNS сообщения
        return p;

    } else { 
        const int len = *p++; // переходим с адреса метки со значением 7 на e(xample) затем на c(om)
        if (p + len + 1 > end) {
            fprintf(stderr, "End of message.\n"); exit(1);}

        printf("%.*s", len, p); // выводим на консоль в виде строки значение памяти начиная с адреса p длинной 7 байт -example,  затем точка, затем 3 байта -com
        p += len;
        if (*p) {
            printf(".");
            return print_name(msg, p, end);
        } else {
            return p+1; // перемещаем указатель в поле NAME секции Question или Answer на нулевой символ конца строки
        }
    }
}

void print_dns_message(const char *message, int msg_length) { // в запросе message(0x22f820 "\n\v\001") это query((char (*)[1024]) 0x22f820) // msg_length - 29
//в ответе message это read, msg_length - 45
    if (msg_length < 12) {
        fprintf(stderr, "Message is too short to be valid.\n");
        exit(1);
    }

    const unsigned char *msg = (const unsigned char *)message;

    /*
    int i;
    for (i = 0; i < msg_length; ++i) {
        unsigned char r = msg[i];
        printf("%02d:   %02X  %03d  '%c'\n", i, r, r, r);
    }
    printf("\n");
    */


    printf("ID = %0X %0X\n", msg[0], msg[1]); // ID = A B // ID:  Произвольный 16-битный идентификатор запроса. Ранее был указан А и В (занимает 16 бит)

    const int qr = (msg[2] & 0x80) >> 7; // QR - поле в секции HEADER // msg[2] это '0000 0001', 0x80 это маска 1000 0000 для третьего байта хедера // если это запрос, то qr равен 0, если ответ, то qr равен 1 // при запросе здесь в третьем байте только бит RD равен 1
    printf("QR = %d %s\n", qr, qr ? "response" : "query"); //бит QR = 0 т.е. это запрос(query)

    const int opcode = (msg[2] & 0x78) >> 3; // тип запросаЮ напр. стандартный, обратный, запрос статуса сервера // 0x78 это маска 0111 1000 для msg[2] т.е. для '0000 0001' третьего байта хедера включаещего QR(1бит), Opcode(4бита), AA(1), TC(1), RD(1), RA(1) // результат применения маски 0000 0000, сдвигаем на 3 бита вправо получаем 0
    printf("OPCODE = %d ", opcode);
    switch(opcode) {
        case 0: printf("standard\n"); break;
        case 1: printf("reverse\n"); break;
        case 2: printf("status\n"); break;
        default: printf("?\n"); break;
    }

	// типы записи - QTYPE
    const int aa = (msg[2] & 0x04) >> 2;
    printf("AA = %d %s\n", aa, aa ? "authoritative" : "");

    const int tc = (msg[2] & 0x02) >> 1;
    printf("TC = %d %s\n", tc, tc ? "message truncated" : "");

    const int rd = (msg[2] & 0x01);
    printf("RD = %d %s\n", rd, rd ? "recursion desired" : "");

    if (qr) { // qr равен 1 если это ответ // QR - поле в разделе HEADER
        const int rcode = msg[3] & 0x07;
        printf("RCODE = %d ", rcode); // RCODE это поле загололвка запроса // RCODE устанавливается в ответе DNS, чтобы указать состояние ошибки
        switch(rcode) {
            case 0: printf("success\n"); break;
            case 1: printf("format error\n"); break;
            case 2: printf("server failure\n"); break;
            case 3: printf("name error\n"); break;
            case 4: printf("not implemented\n"); break;
            case 5: printf("refused\n"); break;
            default: printf("?\n"); break;
        }
        if (rcode != 0) return;
    }

    const int qdcount = (msg[4] << 8) + msg[5]; // QDCOUNT - поле в HEADER, занимает 2 байта у нас это 0x00, 0x01 где 0x00 это msg[4], а 0x01 это msg[5] // qdcount равен 1
    const int ancount = (msg[6] << 8) + msg[7];
    const int nscount = (msg[8] << 8) + msg[9];
    const int arcount = (msg[10] << 8) + msg[11];

    printf("QDCOUNT = %d\n", qdcount); // QDCOUNT: 16-битное беззнаковое целое, определяющее число записей в секции запроса в HEADER . Мы отправляем 1 запрос
    printf("ANCOUNT = %d\n", ancount);
    printf("NSCOUNT = %d\n", nscount);
    printf("ARCOUNT = %d\n", arcount);
    //прочитали HEADER в ответе, размер 12 байт

    const unsigned char *p = msg + 12; // указатель Question // в запросе, msg адрес первого элемента массива query(здесь в функции message) // *p это 7
    const unsigned char *end = msg + msg_length; //end это адрес конца msg // при запрое msg_length - 29 // при ответе msg_length это 45

    if (qdcount) { //QDCOUNT - поле //  число записей в секции запроса
        int i;
        for (i = 0; i < qdcount; ++i) {//QDCOUNT - поле//  число записей в секции запроса или ответа
            if (p >= end) {
                fprintf(stderr, "End of message.\n"); exit(1);}

            printf("Query %2d\n", i + 1); //QDCOUNT - поле
            printf("  name: ");

            // значение p - адрес за секцией Header, за полем NAME в секции Question для запроса(поле QTYPE) и для ответа(поле TYPE)
            // аргумент p указывает на начало поля NAME (p* это 7, p это адрес "7example3com") за полем ID в Header в формате запроса и ответа
            // msg это message целиком; // для запроса - message это query // для ответа message это read (0x22f420)
            p = print_name(msg, p, end); printf("\n");
            if (p + 4 > end) {
                fprintf(stderr, "End of message.\n"); exit(1);}

            const int type = (p[0] << 8) + p[1]; // работаем с битами
            printf("  type: %d\n", type);
            p += 2; // преходим к полю QCLASS (поcле поля NAME) в секции Question // Здесь проверить // если Answer, то к полю TYPE

            const int qclass = (p[0] << 8) + p[1]; // работаем с битами
            printf(" class: %d\n", qclass);
            p += 2; // в Header переходим к следующей секции после Question - к Answer
        }
    }

    if (ancount || nscount || arcount) { //поля в Header - ANCOUNT(количество ответов, включенных в сообщение), NSCOUNT и ARCOUNT указывают количество записей в соответствующих разделах
        int i;
        for (i = 0; i < ancount + nscount + arcount; ++i) {
            if (p >= end) {
                fprintf(stderr, "End of message.\n"); exit(1);}

            printf("Answer %2d\n", i + 1);
            printf("  name: ");

            p = print_name(msg, p, end); printf("\n"); // аргумент p указывает адрес памяти за Header и Question на начало Answer на поле NAME

            if (p + 10 > end) {
                fprintf(stderr, "End of message.\n"); exit(1);}

            const int type = (p[0] << 8) + p[1];
            printf("  type: %d\n", type); // вывели в консоль TYPE
            p += 2; // переходим к полю CLASS в формате ответа

            const int qclass = (p[0] << 8) + p[1];
            printf(" class: %d\n", qclass); // вывели в консоль CLASS
            p += 2; // переходим к полю TTL

            const unsigned int ttl = (p[0] << 24) + (p[1] << 16) +
                (p[2] << 8) + p[3];
            printf("   ttl: %u\n", ttl); // sizeof ttl - 4 байта
            p += 4; // переходим к полю RDLENGTH
            // За TTL следует 16-битный спецификатор длины RDLENGTH, за которым следуют данные. Длина данных составляет RDLENGTH, и интерпретация данных зависит от типа, указанного параметром TYPE.

            const int rdlen = (p[0] << 8) + p[1]; // rdlen - 4 байта
            printf(" rdlen: %d\n", rdlen);
            p += 2; // переход к данным

            if (p + rdlen > end) {
                fprintf(stderr, "End of message.\n"); exit(1);}

            if (rdlen == 4 && type == 1) {
                /* A Record */
                printf("Address "); 
				// выводим в консоль IP адрес // Выводим данные в консоль взависимости от TYPE
                printf("%d.%d.%d.%d\n", p[0], p[1], p[2], p[3]);

            } else if (rdlen == 16 && type == 28) {
                /* AAAA Record */
                printf("Address ");
                int j;
                for (j = 0; j < rdlen; j+=2) {
                    printf("%02x%02x", p[j], p[j+1]);
                    if (j + 2 < rdlen) printf(":");
                }
                printf("\n");

            } else if (type == 15 && rdlen > 3) {
                /* MX Record */
                const int preference = (p[0] << 8) + p[1];
                printf("  pref: %d\n", preference);
                printf("MX: ");
                print_name(msg, p+2, end); printf("\n");

            } else if (type == 16) {
                /* TXT Record */
                printf("TXT: '%.*s'\n", rdlen-1, p+1);

            } else if (type == 5) {
                /* CNAME Record */
                printf("CNAME: ");
                print_name(msg, p, end); printf("\n");
            }

            p += rdlen;
        }
    }

    if (p != end) {
        printf("There is some unread data left over.\n");
    }

    printf("\n");
}


int main(int argc, char *argv[]) { // ---------------------------------

    /* if (argc < 3) {
        printf("Usage:\n\tdns_query hostname type\n");
        printf("Example:\n\tdns_query example.com aaaa\n");
        exit(0);
    }

    if (strlen(argv[1]) > 255) {
        fprintf(stderr, "Hostname too long.");
        exit(1);
    } */


    // unsigned char type;
    unsigned char type = 1;
    /* if (strcmp(argv[2], "a") == 0) {
        type = 1;
    } else if (strcmp(argv[2], "mx") == 0) {
        type = 15;
    } else if (strcmp(argv[2], "txt") == 0) {
        type = 16;
    } else if (strcmp(argv[2], "aaaa") == 0) {
        type = 28;
    } else if (strcmp(argv[2], "any") == 0) {
        type = 255;
    } else {
        fprintf(stderr, "Unknown type '%s'. Use a, aaaa, txt, mx, or any.",
                argv[2]);
        exit(1);
    } */

#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif

    printf("Configuring remote address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    struct addrinfo *peer_address;
    if (getaddrinfo("8.8.8.8", "53", &hints, &peer_address)) {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO()); // #define GETSOCKETERRNO() (WSAGetLastError())
        return 1;
    }

    //int socket(int domain, int type, int protocol); // domain -
    printf("Creating socket...\n");
    SOCKET socket_peer;
    socket_peer = socket(peer_address->ai_family,
            peer_address->ai_socktype, peer_address->ai_protocol); //ai_protocol = 0
    if (!ISVALIDSOCKET(socket_peer)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }


	// формируем запрос // побайтово заполняем массив нужными данными
	// Первые 12 байтов составляют заголовок и известны во время компиляции. Мы можем сохранить их с помощью кода:
	// char query[1024] = {0xAB, 0xCD, /* ID */ // было
	// char query[1024] = {0x0A, 0x0B, // експеримент
    char query[1024] = {0x0E, 0x0F, /* ID */ // експеримент
                        0x01, 0x00, /* Set recursion */ // каждый элемент маасива занимает 1 байт
                        0x00, 0x01, /* QDCOUNT */
                        0x00, 0x00, /* ANCOUNT */
                        0x00, 0x00, /* NSCOUNT */
                        0x00, 0x00 /* ARCOUNT */};
	// Приведенный выше код устанавливает идентификатор нашего запроса на 0xABCD, устанавливает запрос рекурсии и указывает, что мы прикрепляем 1 вопрос. Как упоминалось ранее, 1 - это единственное количество вопросов, поддерживаемых реальными DNS-серверами.

    char *p = query + 12;  // имя массива(query) является адресом его первого элемента // плюс 12 байт - то, что будет в запросе после заголовка
    //char *h = argv[1];
    char *adr = "ya.ru";
    char *h = adr;

    while(*h) {
        char *len = p; // p адрес за концом заголовка // в цикле объявляем переменную указатель len для записи значения метки в массив по адресу метки // сначала адрес метки будет находится за идентификатором запроса, затем будет находится после имени хоста за точкой (example.) перед com // после каждой итерации в while переменная char *len объявляется заново
        p++; // увеличиваем адрес на один байт (после выполнения внутреннего while p указывает на точку в имени хоста)
        //if (h != argv[1]) ++h;
        if (h != adr)
            ++h;  // условие выполнится после выполнения ниже идущего цикла // увеличиваем адрес h для перехода за точку (.com), т.е. на <c>

        while(*h && *h != '.'){
            *p++ = *h++; 
        }

        *len = p - len - 1; // длинна метки, для example это 7, т.е 7example, для com - 3com // присваиваем значение метке(адрес метки находится за идентификатором запроса)
    }
	/*
	Предыдущий код сначала устанавливает новый указатель p в конец заголовка запроса. Мы будем добавлять(имя хоста) в запрос, начиная с р. Мы также определяем указатель h, который мы используем для перебора имени хоста.
	Мы можем выполнить цикл while * h! = 0, потому что * h равно нулю, когда мы закончили читать имя хоста. Внутри цикла мы используем переменную len для хранения позиции начала метки. Значение в этой позиции необходимо установить, чтобы указать длину предстоящей метки. Затем мы копируем символы из * h в * p, пока не найдем точку или конец имени хоста. Если любой из них найден, код устанавливает * len равным длине метки. Затем код переходит к следующей метке.
	Наконец, вне цикла мы добавляем завершающий байт 0, чтобы завершить раздел имени вопроса.
	Затем мы добавляем в запрос тип вопроса и класс вопроса с помощью следующего кода:*/

	*p++ = 0; // добавляем завершающий байт 0, чтобы завершить раздел имени вопроса
    *p++ = 0x00; *p++ = type; /* QTYPE */ // тип записи "А"
    *p++ = 0x00; *p++ = 0x01; /* QCLASS */


    const int query_size = p - query; // query_size равен 29, это 12+17 (загловок(12) +  7example3com(12байт) + нулевой симввол(1байт) + QTYPE(2байта) + QCLASS(2байта))

    int bytes_sent = sendto(socket_peer,
            query, query_size,
            0,
            peer_address->ai_addr, peer_address->ai_addrlen);
    // bytes_sent - 29 байт
    printf("Sent %d bytes.\n", bytes_sent);

    print_dns_message(query, query_size);

    char read[1024];
    int bytes_received = recvfrom(socket_peer, // bytes_received - 45байт
            read, 1024, 0, 0, 0);

    printf("Received %d bytes.\n", bytes_received);

    print_dns_message(read, bytes_received);
    printf("\n");


    freeaddrinfo(peer_address);
    CLOSESOCKET(socket_peer);

#if defined(_WIN32)
    WSACleanup();
#endif

    return 0;
}

