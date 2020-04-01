/* loc_stat.c -- using a local static variable */

// create local variables having block scope but static duration (статическую продолжительность хранения)

#include <stdio.h>
void trystat(void);

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
    static int stay = 10;
    
    printf("fade = %d and stay = %d\n", fade++, stay++);
}

/*
Начинается итерация 1: 
fade = 1 и stay = 1 
Начинается итерация 2 : 
fade = 1 и stay = 2 
Начинается итерация 3: 
fade = 1 и stay = 3
*/
