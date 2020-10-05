/* binary.c -- prints integer in binary form */
// перевод десятичного числва в двоичное
#include <stdio.h>
void to_binary(unsigned long n);

int main(void)
{
    unsigned long number;
    printf("Enter an integer (q to quit):\n");
	// функция scanf() возвращает количество прочитанных ею элементов, поэтому она возвратит 1, если прочитает число или, если указан строковый спецификатор, символы до пробела
    while (scanf("%lu", &number) == 1) // unsigned long - %lu. // выходим из цикла если ввели не число, а символ(т.к. указан спецификатор %lu) 
    {
        printf("Binary equivalent: ");
        to_binary(number); /* например ввели 2*/
        putchar('\n');
        printf("Enter an integer (q to quit):\n");
    }
    printf("Done.\n");
    
    return 0;
}

void to_binary(unsigned long n)   /* например ввели 2*/
{
    int r;
    
    r = n % 2; // 2 % 2 = 0
    if (n >= 2) // да, два равно двум
        to_binary(n / 2); // берем целую часть от деления т.к. тип int, 2 / 2 = 1
     putchar(r == 0 ? '0' : '1'); // после завершения рекурсивного вызова функции вычесленные значения 1 или 0 поочередно выводятся на экран. 
	 // вывод в косоль 10 т.е 2
    
    return;
}
