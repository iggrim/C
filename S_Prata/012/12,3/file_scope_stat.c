// Variables with file scope automatically (and necessarily) have static storage duration
// Переменные с областью видимостью в пределах файла автоматически (и обязательно) имеют статическую продолжительность хранения. 

#include <stdio.h>
void trystat(void);

int stay = 10;

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

void trystat(void)
{
    int fade = 1;
    //static int stay = 10;
    
    printf("fade = %d and stay = %d\n", fade++, stay++);
}

/*
Here comes iteration 1:
fade = 1 and stay = 10
Here comes iteration 2:
fade = 1 and stay = 11
Here comes iteration 3:
fade = 1 and stay = 12
*/
