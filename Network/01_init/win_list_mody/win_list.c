
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600 //  каким объемом Windows API вы собираетесь пользоваться //0x0600 - Windows Vista  
#endif

#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

// If you're compiling with	MinGW, these lines will	have no	effect.	You	will	need to	link to	these libraries	explicitly on the command line, for	example, 
// gcc win_list.c -o win_list.exe -liphlpapi -lws2_32
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

int main() {

    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        printf("Failed to initialize.\n");
        return -1;
    }

/*
	функциz GetAdaptersAddresses возвращает список сетевых интерфейсов в системе. 
	Описание ф-ции:

ULONG WINAPI GetAdaptersAddresses(
  _In_    ULONG                 Family,
  _In_    ULONG                 Flags,
  _In_    PVOID                 Reserved,
  _Inout_ PIP_ADAPTER_ADDRESSES AdapterAddresses,
  _Inout_ PULONG                SizePointer
);
*/
/*
IP_ADAPTER_ADDRESSES *pAddresses; // ====не PIP_ADAPTER_ADDRESSES===
// PIP_ADAPTER_ADDRESSES pAddresses; // другой варииант
outBufLen = sizeof (IP_ADAPTER_ADDRESSES);
pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen) // 
*/
    DWORD asize = 20000;
    PIP_ADAPTER_ADDRESSES adapters;
    do {
        adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);

        if (!adapters) {
            printf("Couldn't allocate %ld bytes for adapters.\n", asize);
            WSACleanup();
            return -1;
        }
	/*
	Для получения информации о параметрах стека TCP/IP можно использовать API-функции. Прототипы функций определены в файле Iphlpapi.h(в том числе и GetAdaptersAddresses). При компоновке приложения необходимо подключать библиотеку Iphlpapi.lib. Для 
	подключения заголовочного файла и библиотеки необходима установка Microsoft Platform SDK (Software Development Kit). 
	*/
	// The GetAdaptersAddresses function retrieves information for IPv4 and IPv6 addresses and returns this information as a linked list of IP_ADAPTER_ADDRESSES structures или *PIP_ADAPTER_ADDRESSES которые указывают на структуру _IP_ADAPTER_ADDRESSES (см. доп.)
	// функциz GetAdaptersAddresses возвращает список сетевых интерфейсов в системе
        int r = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, adapters, &asize);
                
        if (r == ERROR_BUFFER_OVERFLOW) {
            printf("GetAdaptersAddresses wants %ld bytes.\n", asize);
            free(adapters);
        } else if (r == ERROR_SUCCESS) { // On success,	GetAdapterAddresses() returns ERROR_SUCCESS, in which case, we break from the loop and continue. Any	other return value is an error
            break;
        } else {
            printf("Error from GetAdaptersAddresses: %d\n", r);
            free(adapters);
            WSACleanup();
            return -1;
        }
    } while (!adapters); // Использовать цикл do...while лучше в тех случаях, когда должна быть выполнена хотя бы одна итерация, либо когда инициализация объектов, участвующих в проверке условия, происходит внутри тела цикла.

	// When	GetAdapterAddresses() returns successfully, it will	have written a linked list into	adapters with each	adapter's address information.	Our	next step is to	loop through this linked list and print information	for	each adapter and address:
    PIP_ADAPTER_ADDRESSES adapter = adapters; // new variable, adapter, which we use to walk through the linked list of adapters // структура та же

    while (adapter) {
        printf("\nAdapter name: %S\n", adapter->FriendlyName);

        PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;
        while (address) {
            printf("\t%s",
                    address->Address.lpSockaddr->sa_family == AF_INET ?
                    "IPv4" : "IPv6");
			

            char ap[100];

            getnameinfo(address->Address.lpSockaddr,
                    address->Address.iSockaddrLength,
                    ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            printf("\t%s\n", ap);

            address = address->Next;
        }
		
		// есть пример получения MAC адреса https://docs.microsoft.com/en-us/windows/win32/api/iptypes/ns-iptypes-ip_adapter_addresses_lh		
		printf("\nMAC address: %.2X\n", (int)adapter->PhysicalAddress[0]);  // получил (у меня) 74 для первого элемента из шести - все верно. Надо сделать цикл
		
		// меняем МАК адрес, пишем в первый элемент массива новое значение 51
		adapter->PhysicalAddress[0] =51; // 51 в десятично системе счисления, это 33 в шестнадцатиричиной.
		
        printf("\nMAC address: %.2X\n", (int)adapter->PhysicalAddress[0]); //получил 33 - все верно. 
		// Уровень драйвера в приоритете, в окне настроек адаптера попрежнему первое число из шести 74
		// В цикле for (i = 0; i < adapter->PhysicalAddressLength;i++) можно вывести  полный MAC адрес пройдя по массиву PhysicalAddress (см. структуру PIP_ADAPTER_ADDRESSES)

        adapter = adapter->Next;
    }


    free(adapters);
    WSACleanup();
    return 0;
}
/*
C++

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
*/

/*
C++

typedef struct _IP_ADAPTER_UNICAST_ADDRESS_LH {
  union {
    ULONGLONG Alignment;
    struct {
      ULONG Length;
      DWORD Flags;
    };
  };
  struct _IP_ADAPTER_UNICAST_ADDRESS_LH *Next;
  SOCKET_ADDRESS                        Address;
  IP_PREFIX_ORIGIN                      PrefixOrigin;
  IP_SUFFIX_ORIGIN                      SuffixOrigin;
  IP_DAD_STATE                          DadState;
  ULONG                                 ValidLifetime;
  ULONG                                 PreferredLifetime;
  ULONG                                 LeaseLifetime;
  UINT8                                 OnLinkPrefixLength;
} IP_ADAPTER_UNICAST_ADDRESS_LH, *PIP_ADAPTER_UNICAST_ADDRESS_LH;
*/