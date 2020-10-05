/* binary.c -- prints integer in binary form */
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
        to_binary(number);
        putchar('\n');
        printf("Enter an integer (q to quit):\n");
    }
    printf("Done.\n");

    return 0;
}

void to_binary(unsigned long n)   /* recursive function */
{
    int r;

    r = n % 2;
    if (n >= 2)
        to_binary(n / 2);
     putchar(r == 0 ? '0' : '1');

    return;
}
