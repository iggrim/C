#include <stdio.h>
// void trystat(void); // указывать прототип не обязательно, поскольку функция в этом файле определена, но (здесь) не вызывается.

void trystat(void)
{
    int fade = 1;
	extern int stay; // объявляем переменную stay как внешнюю // определение в file_scope_stat.c
    
    printf("fade = %d and stay = %d\n", fade++, stay++);
}