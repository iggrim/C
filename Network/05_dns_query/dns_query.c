#include "chap05.h"

// msg - ��������� �� ������ ���������(��� ��� ���������� ��� ������ �� ����� ������), p - ��������� �� ���, ������� ����� ����������,  end - ��������� �� ������ �� ������ ���������. end ���������, ����� �� ����� ���������, �� ������ �� �� ����� ����� ����������� ���������.
const unsigned char *print_name(const unsigned char *msg, const unsigned char *p,
      const unsigned char *end) {

    if (p + 2 > end) {
        fprintf(stderr, "End of message.\n");
		exit(1);
        }

    // DNS �������� ����� ������ �������. ������ ������ ����� ������������ �� ������, �� ������� ������� �� �����.
    // ����� ��������� ��������� �����, � ����� ��� ����������� ����� ������ 0.
    // ���� ��� ����� ����������� ��� ������� ���� (�� ���� 0xc0), ����� ��� � ��������� ���� ������ ������������������ ��� ���������.
    if ((*p & 0xC0) == 0xC0) { 
        const int k = ((*p & 0x3F) << 8) + p[1]; // 0x3F ��� 0011.1111 // �������� K ��� 12
        p += 2;
        printf(" (pointer %d) ", k);
        print_name(msg, msg+k, end); // end ��� �������� ������� DNS ���������
        return p;

    } else { 
        const int len = *p++; // ��������� � ������ ����� �� ��������� 7 �� e(xample) ����� �� c(om)
        if (p + len + 1 > end) {
            fprintf(stderr, "End of message.\n"); exit(1);}

        printf("%.*s", len, p); // ������� �� ������� � ���� ������ �������� ������ ������� � ������ p ������� 7 ���� -example,  ����� �����, ����� 3 ����� -com
        p += len;
        if (*p) {
            printf(".");
            return print_name(msg, p, end);
        } else {
            return p+1; // ���������� ��������� � ���� NAME ������ Question ��� Answer �� ������� ������ ����� ������
        }
    }
}

void print_dns_message(const char *message, int msg_length) { // � ������� message(0x22f820 "\n\v\001") ��� query((char (*)[1024]) 0x22f820) // msg_length - 29
//� ������ message ��� read, msg_length - 45
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


    printf("ID = %0X %0X\n", msg[0], msg[1]); // ID = A B // ID:  ������������ 16-������ ������������� �������. ����� ��� ������ � � � (�������� 16 ���)

    const int qr = (msg[2] & 0x80) >> 7; // QR - ���� � ������ HEADER // msg[2] ��� '0000 0001', 0x80 ��� ����� 1000 0000 ��� �������� ����� ������ // ���� ��� ������, �� qr ����� 0, ���� �����, �� qr ����� 1 // ��� ������� ����� � ������� ����� ������ ��� RD ����� 1
    printf("QR = %d %s\n", qr, qr ? "response" : "query"); //��� QR = 0 �.�. ��� ������(query)

    const int opcode = (msg[2] & 0x78) >> 3; // ��� �������� ����. �����������, ��������, ������ ������� ������� // 0x78 ��� ����� 0111 1000 ��� msg[2] �.�. ��� '0000 0001' �������� ����� ������ ����������� QR(1���), Opcode(4����), AA(1), TC(1), RD(1), RA(1) // ��������� ���������� ����� 0000 0000, �������� �� 3 ���� ������ �������� 0
    printf("OPCODE = %d ", opcode);
    switch(opcode) {
        case 0: printf("standard\n"); break;
        case 1: printf("reverse\n"); break;
        case 2: printf("status\n"); break;
        default: printf("?\n"); break;
    }

	// ���� ������ - QTYPE
    const int aa = (msg[2] & 0x04) >> 2;
    printf("AA = %d %s\n", aa, aa ? "authoritative" : "");

    const int tc = (msg[2] & 0x02) >> 1;
    printf("TC = %d %s\n", tc, tc ? "message truncated" : "");

    const int rd = (msg[2] & 0x01);
    printf("RD = %d %s\n", rd, rd ? "recursion desired" : "");

    if (qr) { // qr ����� 1 ���� ��� ����� // QR - ���� � ������� HEADER
        const int rcode = msg[3] & 0x07;
        printf("RCODE = %d ", rcode); // RCODE ��� ���� ���������� ������� // RCODE ��������������� � ������ DNS, ����� ������� ��������� ������
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

    const int qdcount = (msg[4] << 8) + msg[5]; // QDCOUNT - ���� � HEADER, �������� 2 ����� � ��� ��� 0x00, 0x01 ��� 0x00 ��� msg[4], � 0x01 ��� msg[5] // qdcount ����� 1
    const int ancount = (msg[6] << 8) + msg[7];
    const int nscount = (msg[8] << 8) + msg[9];
    const int arcount = (msg[10] << 8) + msg[11];

    printf("QDCOUNT = %d\n", qdcount); // QDCOUNT: 16-������ ����������� �����, ������������ ����� ������� � ������ ������� � HEADER . �� ���������� 1 ������
    printf("ANCOUNT = %d\n", ancount);
    printf("NSCOUNT = %d\n", nscount);
    printf("ARCOUNT = %d\n", arcount);
    //��������� HEADER � ������, ������ 12 ����

    const unsigned char *p = msg + 12; // ��������� Question // � �������, msg ����� ������� �������� ������� query(����� � ������� message) // *p ��� 7
    const unsigned char *end = msg + msg_length; //end ��� ����� ����� msg // ��� ������ msg_length - 29 // ��� ������ msg_length ��� 45

    if (qdcount) { //QDCOUNT - ���� //  ����� ������� � ������ �������
        int i;
        for (i = 0; i < qdcount; ++i) {//QDCOUNT - ����//  ����� ������� � ������ ������� ��� ������
            if (p >= end) {
                fprintf(stderr, "End of message.\n"); exit(1);}

            printf("Query %2d\n", i + 1); //QDCOUNT - ����
            printf("  name: ");

            // �������� p - ����� �� ������� Header, �� ����� NAME � ������ Question ��� �������(���� QTYPE) � ��� ������(���� TYPE)
            // �������� p ��������� �� ������ ���� NAME (p* ��� 7, p ��� ����� "7example3com") �� ����� ID � Header � ������� ������� � ������
            // msg ��� message �������; // ��� ������� - message ��� query // ��� ������ message ��� read (0x22f420)
            p = print_name(msg, p, end); printf("\n");
            if (p + 4 > end) {
                fprintf(stderr, "End of message.\n"); exit(1);}

            const int type = (p[0] << 8) + p[1]; // �������� � ������
            printf("  type: %d\n", type);
            p += 2; // �������� � ���� QCLASS (��c�� ���� NAME) � ������ Question // ����� ��������� // ���� Answer, �� � ���� TYPE

            const int qclass = (p[0] << 8) + p[1]; // �������� � ������
            printf(" class: %d\n", qclass);
            p += 2; // � Header ��������� � ��������� ������ ����� Question - � Answer
        }
    }

    if (ancount || nscount || arcount) { //���� � Header - ANCOUNT(���������� �������, ���������� � ���������), NSCOUNT � ARCOUNT ��������� ���������� ������� � ��������������� ��������
        int i;
        for (i = 0; i < ancount + nscount + arcount; ++i) {
            if (p >= end) {
                fprintf(stderr, "End of message.\n"); exit(1);}

            printf("Answer %2d\n", i + 1);
            printf("  name: ");

            p = print_name(msg, p, end); printf("\n"); // �������� p ��������� ����� ������ �� Header � Question �� ������ Answer �� ���� NAME

            if (p + 10 > end) {
                fprintf(stderr, "End of message.\n"); exit(1);}

            const int type = (p[0] << 8) + p[1];
            printf("  type: %d\n", type); // ������ � ������� TYPE
            p += 2; // ��������� � ���� CLASS � ������� ������

            const int qclass = (p[0] << 8) + p[1];
            printf(" class: %d\n", qclass); // ������ � ������� CLASS
            p += 2; // ��������� � ���� TTL

            const unsigned int ttl = (p[0] << 24) + (p[1] << 16) +
                (p[2] << 8) + p[3];
            printf("   ttl: %u\n", ttl); // sizeof ttl - 4 �����
            p += 4; // ��������� � ���� RDLENGTH
            // �� TTL ������� 16-������ ������������ ����� RDLENGTH, �� ������� ������� ������. ����� ������ ���������� RDLENGTH, � ������������� ������ ������� �� ����, ���������� ���������� TYPE.

            const int rdlen = (p[0] << 8) + p[1]; // rdlen - 4 �����
            printf(" rdlen: %d\n", rdlen);
            p += 2; // ������� � ������

            if (p + rdlen > end) {
                fprintf(stderr, "End of message.\n"); exit(1);}

            if (rdlen == 4 && type == 1) {
                /* A Record */
                printf("Address "); 
				// ������� � ������� IP ����� // ������� ������ � ������� ������������ �� TYPE
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


	// ��������� ������ // ��������� ��������� ������ ������� �������
	// ������ 12 ������ ���������� ��������� � �������� �� ����� ����������. �� ����� ��������� �� � ������� ����:
	// char query[1024] = {0xAB, 0xCD, /* ID */ // ����
	// char query[1024] = {0x0A, 0x0B, // �����������
    char query[1024] = {0x0E, 0x0F, /* ID */ // �����������
                        0x01, 0x00, /* Set recursion */ // ������ ������� ������� �������� 1 ����
                        0x00, 0x01, /* QDCOUNT */
                        0x00, 0x00, /* ANCOUNT */
                        0x00, 0x00, /* NSCOUNT */
                        0x00, 0x00 /* ARCOUNT */};
	// ����������� ���� ��� ������������� ������������� ������ ������� �� 0xABCD, ������������� ������ �������� � ���������, ��� �� ����������� 1 ������. ��� ����������� �����, 1 - ��� ������������ ���������� ��������, �������������� ��������� DNS-���������.

    char *p = query + 12;  // ��� �������(query) �������� ������� ��� ������� �������� // ���� 12 ���� - ��, ��� ����� � ������� ����� ���������
    //char *h = argv[1];
    char *adr = "ya.ru";
    char *h = adr;

    while(*h) {
        char *len = p; // p ����� �� ������ ��������� // � ����� ��������� ���������� ��������� len ��� ������ �������� ����� � ������ �� ������ ����� // ������� ����� ����� ����� ��������� �� ��������������� �������, ����� ����� ��������� ����� ����� ����� �� ������ (example.) ����� com // ����� ������ �������� � while ���������� char *len ����������� ������
        p++; // ����������� ����� �� ���� ���� (����� ���������� ����������� while p ��������� �� ����� � ����� �����)
        //if (h != argv[1]) ++h;
        if (h != adr)
            ++h;  // ������� ���������� ����� ���������� ���� ������� ����� // ����������� ����� h ��� �������� �� ����� (.com), �.�. �� <c>

        while(*h && *h != '.'){
            *p++ = *h++; 
        }

        *len = p - len - 1; // ������ �����, ��� example ��� 7, �.� 7example, ��� com - 3com // ����������� �������� �����(����� ����� ��������� �� ��������������� �������)
    }
	/*
	���������� ��� ������� ������������� ����� ��������� p � ����� ��������� �������. �� ����� ���������(��� �����) � ������, ������� � �. �� ����� ���������� ��������� h, ������� �� ���������� ��� �������� ����� �����.
	�� ����� ��������� ���� while * h! = 0, ������ ��� * h ����� ����, ����� �� ��������� ������ ��� �����. ������ ����� �� ���������� ���������� len ��� �������� ������� ������ �����. �������� � ���� ������� ���������� ����������, ����� ������� ����� ����������� �����. ����� �� �������� ������� �� * h � * p, ���� �� ������ ����� ��� ����� ����� �����. ���� ����� �� ��� ������, ��� ������������� * len ������ ����� �����. ����� ��� ��������� � ��������� �����.
	�������, ��� ����� �� ��������� ����������� ���� 0, ����� ��������� ������ ����� �������.
	����� �� ��������� � ������ ��� ������� � ����� ������� � ������� ���������� ����:*/

	*p++ = 0; // ��������� ����������� ���� 0, ����� ��������� ������ ����� �������
    *p++ = 0x00; *p++ = type; /* QTYPE */ // ��� ������ "�"
    *p++ = 0x00; *p++ = 0x01; /* QCLASS */


    const int query_size = p - query; // query_size ����� 29, ��� 12+17 (��������(12) +  7example3com(12����) + ������� �������(1����) + QTYPE(2�����) + QCLASS(2�����))

    int bytes_sent = sendto(socket_peer,
            query, query_size,
            0,
            peer_address->ai_addr, peer_address->ai_addrlen);
    // bytes_sent - 29 ����
    printf("Sent %d bytes.\n", bytes_sent);

    print_dns_message(query, query_size);

    char read[1024];
    int bytes_received = recvfrom(socket_peer, // bytes_received - 45����
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

