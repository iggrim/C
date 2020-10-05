#include <stdio.h>
// void trystat(void); // указывать прототип не обязательно, поскольку функция в файле определена, но здесь не вызывается.

extern int stay; // Для порядка лучше все статические переменные с внешним связыванием объявлять в заголовочных файлах

void trystat(void)
{
    int fade = 1;

    printf("fade = %d and stay = %d\n", fade++, stay++);
}