#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA d;

/*
Для инициализации Winsock вызываем функцию WSAStartup int WSAStartup( WORD wVersReq, (in) LPWSADATA lpWSAData (out) ); Параметр WORD wVersReq - младший  байт - версия, старший байт - под.версия, интерфейса Winsock. Возможные версии - 1.0, 1.1, 2.0, 2.2... Для "сборки" этого параметра используем  макрос MAKEWORD. Например: MAKEWORD (1, 1) - версия 1.1. Более поздние версии отличаются наличием новых функций и механизмов расширений. Параметр lpWSAData - указатель на структуру WSADATA. При  возврате из функции данная структура содержит информацию о проинициализированной нами версии WinsockAPI. 
*/
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        printf("Failed to initialize.\n");
        return -1;
    }

    WSACleanup();

    printf("Ok.\n");

    return 0;
}
/*If you are using Microsoft Visual C as your compiler, then 
-----#pragma comment(lib, "ws2_32.lib") ------- tells Microsoft Visual C to link the executable with the Winsock library, ws2_32.lib.
If you are using MinGW as your compiler, -----the pragma is ignored-----. You need to explicitly tell the compiler to link in the library by adding the command-line option, -lws2_32. For example, you can compile this program using MinGW with the following command:
gcc win_init.c -o win_init.exe -lws2_32
*/