// Variables with file scope automatically (and necessarily) have static storage duration

#include <stdio.h>
// extern void trystat(void); // объявляем trystat как внешнюю функцию // trystat определяется в другом файле

void trystat(void); // можно без extern

int stay = 10;// Переменные с областью видимостью в пределах файла автоматически (и обязательно) имеют статическую продолжительность хранения. 

int main(void)
{
    int count;
    
    for (count = 1; count <= 3; count++)
    {
        printf("Here comes iteration %d:\n", count);
        trystat();
    }
    
    return 0;
}

/* 
Here comes iteration 1:
fade = 1 and stay = 10
Here comes iteration 2:
fade = 1 and stay = 11
Here comes iteration 3:
fade = 1 and stay = 12
*/
