// gcc	unix_list.c	-o	unix_list

#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>


int main() {

    struct ifaddrs *addresses;

	/*
	Функция getifaddrs() создаёт связный список структур, описывающих  
сетевые интерфейсы   локальной системы, и сохраняет адрес первого  
элемента списка в *ifap. Список состоит из структур ifaddrs:
	*/
    if (getifaddrs(&addresses) == -1) {
        printf("getifaddrs call failed\n");
        return -1;
    }

    struct ifaddrs *address = addresses;
    while(address) {
        int family = address->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6) {

            printf("%s\t", address->ifa_name);
            printf("%s\t", family == AF_INET ? "IPv4" : "IPv6");

            char ap[100];
            const int family_size = family == AF_INET ?
                sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
            getnameinfo(address->ifa_addr,
                    family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            printf("\t%s\n", ap);

        }
        address = address->ifa_next;
    }


    freeifaddrs(addresses);
    return 0;
}

/*
Функция getifaddrs() создаёт связный список структур, описывающих сетевые интерфейсы локальной системы, и сохраняет адрес первого элемента списка в *ifap. Список состоит из структур ifaddrs:

struct ifaddrs {
    struct ifaddrs  *ifa_next;    /* след. элемент в списке 
    char            *ifa_name;    /* имя интерфейса 
    unsigned int     ifa_flags;   /* флаги из SIOCGIFFLAGS 
    struct sockaddr *ifa_addr;    /* адрес интерфейса 
    struct sockaddr *ifa_netmask; /* сетевая маска интерфейса 
    union {
        struct sockaddr *ifu_broadaddr;
                         /* широковещательный адрес интерфейса 
        struct sockaddr *ifu_dstaddr;
                         /* адрес назначения точка-точка 
    } ifa_ifu;
#define              ifa_broadaddr ifa_ifu.ifu_broadaddr
#define              ifa_dstaddr   ifa_ifu.ifu_dstaddr
    void            *ifa_data;    /* спец. данные для адреса 
};

В поле ifa_next содержится указатель на следующую структуру в списке или NULL, если это последний элемент в списке.

Поле ifa_name указывает на имя интерфейса (заканчивающееся null).

В поле ifa_flags содержатся флаги интерфейса, полученные операцией SIOCGIFFLAGS ioctl(2) (список флагов приведён в netdevice(7)).

Поле ifa_addr указывает на структуру, содержащую адрес интерфейса (для определения формата структуры адреса обратитесь к подполю sa_family). Это поле может содержать указатель null.

Поле ifa_netmask указывает на структуру, содержащую маску сети для ifa_addr (если она используется в адресном семействе). Это поле может содержать указатель null.

В зависимости от наличия флага IFF_BROADCAST или IFF_POINTOPOINT в ifa_flags (может быть установлен какой-то один), в ifa_broadaddr будет содержаться широковещательный адрес ifa_addr (если он используется в адресном семействе) или ifa_dstaddr будет содержать адрес назначения интерфейса типа точка-точка.

Поле ifa_data указывает на буфер, содержащий данные, присущие адресному семейству; это поле может быть равно NULL, если данных для этого интерфейса нет.

Память под структуру данных, возвращаемая getifaddrs(), выделяется динамически и должна освобождаться с помощью freeifaddrs(), когда больше не нужна. 
*/
