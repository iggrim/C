// Variables with file scope automatically (and necessarily) have static storage duration
#include <stdio.h>

extern void trystat(void); // объявляем trystat как внешнюю функцию // обязательное объявление, если trystat определяется в другом файле
int stay = 10;

int main(void)
{
    int count;
    
    for (count = 1; count <= 3; count++)
    {
        printf("Here comes iteration %d:\n", count);
        trystat();
    }
	
	printf("After iteration %d:\n", stay); // все та же переменная stay измененная измененная в файле ext_func.c
    
    return 0;
}

/* 
Here comes iteration 1:
fade = 1 and stay = 10
Here comes iteration 2:
fade = 1 and stay = 11
Here comes iteration 3:
fade = 1 and stay = 12
After iteration 13:  // все та же переменная stay измененная измененная в файле ext_func.c 
*/
