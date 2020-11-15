#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif
/*
Макрос _WIN32_WINNT должен  быть  определен  первым, чтобы  была  включена  правильная  версия  заголовков  Windows. winsock2.h, iphlpapi.h и ws2tcpip.h - это  заголовки Windows, которые  нам  нужны  для  составления  списка  сетевых  адаптеров. 

#define _WIN32_WINNT 0x0****
Это версия Windows, которая позволяет сообщить компилятору каким объемом Windows API вы собираетесь пользоваться. Когда переходят с одного ядра Windows на другое, появляются новые функции Windows API, которые на предыдущей версии не доступны.
Windows 7 -  _WIN32_WINNT_WIN7  (0x0601)
*/

#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

/*If you are using Microsoft Visual C as your compiler, then 
-----#pragma comment(lib, "ws2_32.lib") ------- tells Microsoft Visual C to link the executable with the Winsock library, ws2_32.lib.
If you are using MinGW as your compiler, -----the pragma is ignored-----. You need to explicitly tell the compiler to link in the library by adding the command-line option, -lws2_32.*/
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
 
/*
-lws2_32 подключает библиотеку C:\Windows\System32\ ws2_32.dll 
-liphlpapi подключает библиотеку C:\Windows\System32\ IPHLPAPI.DLL
*/


int main() {

    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) { // инициализация
        printf("Failed to initialize.\n");
        return -1;
    }


    DWORD asize = 20000;
    PIP_ADAPTER_ADDRESSES adapters;
    do {
        adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);

        if (!adapters) {
            printf("Couldn't allocate %ld bytes for adapters.\n", asize);
            WSACleanup(); // WSAStartup и WSACleanup необходимо вызывать в обязательном порядке
            return -1;
        }

		// функция GetAdaptersAddresses возвращает список сетевых интерфейсов в системе
        // it will have written a linked list into --adapters-- with each adapter's address information.
		/*
		ULONG WINAPI GetAdaptersAddresses(
		   ULONG                 Family,
		   ULONG                 Flags,
		   PVOID                 Reserved,
		   PIP_ADAPTER_ADDRESSES AdapterAddresses,
		   PULONG                SizePointer
		);
		*/
		int r = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0,
                adapters, &asize); // The first	parameter, AF_UNSPEC, tells	Windows	that we	want both IPv4 and IPv6	addresses.
        if (r == ERROR_BUFFER_OVERFLOW) {
            printf("GetAdaptersAddresses wants %ld bytes.\n", asize);
            free(adapters);
        } else if (r == ERROR_SUCCESS) {
            break;  // если все хорошо, то выходим из цикла
        } else {
            printf("Error from GetAdaptersAddresses: %d\n", r);
            free(adapters);
            WSACleanup();
            return -1;
        }
    } while (!adapters);


    PIP_ADAPTER_ADDRESSES adapter = adapters;
    while (adapter) {
        printf("\nAdapter name: %S\n", adapter->FriendlyName); // get adapter name

        PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;  // Первый адрес для каждого адаптера находится в FirstUnicastAddress, в члене структуры PIP_ADAPTER_UNICAST_ADDRESS, которая в совою очередь является членом структуры PIP_ADAPTER_ADDRESSES
        
		while (address) {
            printf("\t%s",
                    address->Address.lpSockaddr->sa_family == AF_INET ?
                    "IPv4" : "IPv6");
					
			// В цикле for (i = 0; i < adapter->PhysicalAddressLength;i++) если надо, то можно вывести  полный MAC адрес пройдя по массиву PhysicalAddress (см. структуру PIP_ADAPTER_ADDRESSES)

            char ap[100];  // выделяем буфер для хранения текстового представления адреса
			
			// Функция getnameinfo() вызывается для преобразования адреса в адрес стандартной записи.
			// Функция getnameinfo() предназначена для перевода сетевого адреса в имя машины, способом, который не зависит от сетевого протокола. Она сочетает в себе действия функций gethostbyaddr() и getservbyport(), и является функцией обратной функции getaddrinfo()
			/*
			int getnameinfo(
			  const SOCKADDR *pSockaddr,
			  socklen_t      SockaddrLength,
			  PCHAR          pNodeBuffer,
			  DWORD          NodeBufferSize,
			  PCHAR          pServiceBuffer,
			  DWORD          ServiceBufferSize,
			  INT            Flags
			);
			*/
            getnameinfo(address->Address.lpSockaddr, // структура SOCKET_ADDRESS как член структуры PIP_ADAPTER_UNICAST_ADDRESS
                    address->Address.iSockaddrLength,
                    ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            printf("\t%s\n", ap);

            address = address->Next; // перемещаемся вперед по связаному списку
        }

        adapter = adapter->Next;
    }


    free(adapters);
    WSACleanup();
    return 0;
}

/*
При установке или получении параметров сокета на IPPROTO_IP уровне приложения C/C++ Winsock, предназначенного для Windows, включение правильного заголовка и файла библиотеки в проект программы имеет решающее значение. Если файлы заголовка и библиотеки не совпадают должным образом setsockopt или getsockopt могут завершиться ошибкой выполнения 10042 (WSAENOPROTOOPT). В некоторых случаях, даже если API возвращается успешно, значение параметра, которое вы устанавливаете или получаете, может не соответствовать вашим ожиданиям.

Чтобы избежать этой проблемы, соблюдайте следующие правила:

    Программа, которая включает, Winsock.h должна связываться только с Wsock32.lib.
    Программа, которая включает в себя Ws2tcpip.h может связать либо с Ws2_32.lib или Wsock32.lib.

    Ws2tcpip.h должен быть явно включен после Winsock2.h, чтобы использовать параметры сокета на этом уровне.
*/

/*
typedef struct _IP_ADAPTER_ADDRESSES {
    union {
     ULONGLONG Alignment;
     struct {
      ULONG Length;
      DWORD IfIndex;
     };
    };
 PCHAR AdapterName;
 PIP_ADAPTER_UNICAST_ADDRESS FirstUnicastAddress;
 PIP_ADAPTER_ANYCAST_ADDRESS FirstAnycastAddress;  // Not supported
 PIP_ADAPTER_MULTICAST_ADDRESS FirstMulticastAddress;  // Not supported
 PIP_ADAPTER_DNS_SERVER_ADDRESS FirstDnsServerAddress;  // Not supported
 PWCHAR DnsSuffix;      // Not supported
 PWCHAR Description;
 PWCHAR FriendlyName;
 BYTE PhysicalAddress[MAX_ADAPTER_ADDRESS_LENGTH];
 DWORD PhysicalAddressLength;
 DWORD Flags;
 DWORD Mtu;
 DWORD IfType;
 IF_OPER_STATUS OperStatus;
 DWORD Ipv6IfIndex;
 DWORD ZoneIndices[16];
 PIP_ADAPTER_PREFIX FirstPrefix;     // Not supported
 ULONG64 TransmitLinkSpeed;     // Not supported
 ULONG64 ReceiveLinkSpeed;     // Not supported
 PIP_ADAPTER_WINS_SERVER_ADDRESS_LH FirstWinsServerAddress; // Not supported
 PIP_ADAPTER_GATEWAY_ADDRESS_LH FirstGatewayAddress;
 ULONG Ipv4Metric;      // Not supported
 ULONG Ipv6Metric;      // Not supported
 IF_LUID Luid;       // Not supported
 SOCKET_ADDRESS Dhcpv4Server;     // Not supported
 NET_IF_COMPARTMENT_ID CompartmentId;    // Not supported
 NET_IF_NETWORK_GUID NetworkGuid;    // Not supported
 NET_IF_CONNECTION_TYPE ConnectionType;
 TUNNEL_TYPE TunnelType;      // Not supported
 SOCKET_ADDRESS Dhcpv6Server;     // Not supported
 BYTE Dhcpv6ClientDuid[MAX_DHCPV6_DUID_LENGTH];   // Not supported
 ULONG Dhcpv6ClientDuidLength;     // Not supported
 ULONG Dhcpv6Iaid;      // Not supported
 PIP_ADAPTER_DNS_SUFFIX FirstDnsSuffix;    // Not supported
} IP_ADAPTER_ADDRESSES, *PIP_ADAPTER_ADDRESSES;

Функция GetAdaptersAddresses извлекает информацию для адресов IPv4 и IPv6 и возвращает эту информацию в виде связанного списка структур IP_ADAPTER_ADDRESSE 
*/

/*
IP_ADAPTER_UNICAST_ADDRESS
The IP_ADAPTER_UNICAST_ADDRESS structure stores a single unicast IP address in a linked list of IP addresses for a 
particular adapter.
Syntax

typedef struct _IP_ADAPTER_UNICAST_ADDRESS {
    union {
     struct {
      ULONG Length;
      DWORD Flags;       // Not supported
     };
    };
	
 struct _IP_ADAPTER_UNICAST_ADDRESS  *Next;
 SOCKET_ADDRESS Address;
 IP_PREFIX_ORIGIN PrefixOrigin;
 IP_SUFFIX_ORIGIN SuffixOrigin;
 IP_DAD_STATE DadState;
 ULONG ValidLifetime;
 ULONG PreferredLifetime;
 ULONG LeaseLifetime;
 UINT8 OnLinkPrefixLength;
} IP_ADAPTER_UNICAST_ADDRESS, 
*PIP_ADAPTER_UNICAST_ADDRESS;

/*
typedef struct _SOCKET_ADDRESS {
     LPSOCKADDR lpSockaddr;
     INT        iSockaddrLength;
} SOCKET_ADDRESS,  
*PSOCKET_ADDRESS, 
*LPSOCKET_ADDRESS
*/

/*
int getnameinfo(
  const SOCKADDR *pSockaddr,
  socklen_t      SockaddrLength,
  PCHAR          pNodeBuffer,
  DWORD          NodeBufferSize,
  PCHAR          pServiceBuffer,
  DWORD          ServiceBufferSize,
  INT            Flags
);

Параметры

pSockaddr
Указатель на структуру адреса сокета, которая содержит адрес и номер порта сокета. Для IPv4 параметр sa указывает на структуру sockaddr_in . Для IPv6 параметр sa указывает на структуру sockaddr_in6 .

SockaddrLength
Длина в байтах структуры, на которую указывает параметр sa .

pNodeBuffer
Указатель на строку ANSI, используемую для хранения имени хоста. В случае успеха имя хоста по умолчанию возвращается как полное доменное имя (FQDN). Если параметр хоста равен ПУСТО ( NULL) , это означает, что вызывающий абонент не хочет получать строку имени хоста.

NodeBufferSize
Длина в байтах буфера, на который указывает параметр хоста . Вызывающий должен предоставить буфер, достаточно большой, чтобы вместить имя хоста, включая завершающий символ NULL .

pServiceBuffer
Указатель на строку ANSI для хранения имени службы. В случае успеха возвращается строка ANSI, представляющая имя службы, связанной с номером порта. Если параметр serv имеет значение NULL , это означает, что вызывающий абонент не хочет получать строку с именем службы.

ServiceBufferSize
Длина в байтах буфера, на который указывает параметр serv . Вызывающий должен предоставить буфер, достаточно большой для хранения имени службы, включая завершающий символ NULL .

Flags
Значение, используемое для настройки обработки функции getnameinfo . См. Раздел 

Возвращаемое значение
При успешном завершении возвращается 0, имя узла и имя сервиса
*/


