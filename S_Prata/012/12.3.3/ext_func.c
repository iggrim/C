#include <stdio.h>
void trystat(void); // указывать прототип не обязательно, поскольку функция в файле определена, но здесь не вызывается.

void trystat(void)
{
    int fade = 1;
     static int stay = 10;
    printf("fade = %d and stay = %d\n", fade++, stay++);
}