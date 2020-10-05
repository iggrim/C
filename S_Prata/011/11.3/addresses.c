//  addresses.c  -- addresses of strings
#define MSG "I'm special." // cтроковый литерал // заключенная в кавычки строка находится в статической памяти. Однако память под массив выделяется только после того, как программа начнет выполнение. В это время строка, заключенная в кавычки, копируется в массив. 

#include <stdio.h>
int main()
{
    char ar[] = MSG;// инициализация массива приводит к копированию строки из статической памяти в массив
    const char *pt = MSG; //переменная pt указывает на первый символ строки MSG
    printf("address of \"I'm special\": %p \n", "I'm special");
    printf("              address ar: %p\n", ar);
    printf("              address pt: %p\n", pt);
    printf("              first simb. pt: %c\n", *pt);
    printf("              next simb. pt: %c\n", *(pt+1));
    printf("              next simb. pt: %c\n", *(pt+2));
    printf("          address of MSG: %p\n", MSG);
    printf("address of \"I'm special\": %p \n", "I'm special");

    return 0;
}
/*
address of "I'm special": 000000000040400D
              address ar: 000000000022FE3B
              address pt: 0000000000404000
              first simb. pt: I
              next simb. pt: '
              next simb. pt: m
          address of MSG: 0000000000404000
address of "I'm special": 000000000040400D
*/