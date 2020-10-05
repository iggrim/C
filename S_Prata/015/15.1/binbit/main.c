/* binbit.c -- using bit operations to display binary */
#include <stdio.h>
#include <limits.h>  // for CHAR_BIT, # of bits per char
char * itobs(int, char *);
void show_bstr(const char *);

int main(void)
{
    // CHAR_BIT из заголовочного файла limits.h. //  макрос представляет количество битов в типе char
    char bin_str[CHAR_BIT * sizeof(int) + 1]; // sizeof(int) = 4 (байта ) // CHAR_BIT = 8 бит
    // т.е. bin_str{32}
    int number;

    puts("Enter integers and see them in binary.");
    puts("Non-numeric input terminates program.");
    while (scanf("%d", &number) == 1)
    {
        itobs(number,bin_str);
        printf("%d is ", number);
        show_bstr(bin_str);
        putchar('\n');
    }
    puts("Bye!");

    return 0;
}

char * itobs(int n, char * ps)// например ввели 25 это 11001ышяу
{
    int i;
    const static int size = CHAR_BIT * sizeof(int); // size=32 (8х4=32)

    for (i = size - 1; i >= 0; i--, n >>= 1)// сдвигаем вправо 1101 на еденицу, получаем 1100 т.е. 12
        ps[i] = (01 & n) + '0'; // результатом 01 & n будет значение последнего бита в n.
        //Значением является О или 1, но для массива необходим символ '0' или символ '1.
        //Преобразование осуществляется добавлением кода для '0'

    ps[size] = '\0';

    return ps;
}

/* show binary string in blocks of 4 */
void show_bstr(const char * str)
{ // функция show_bstr (), которая разбивает последовательность битов на группы по четыре, чтобы облегчить восприятие строки.
    int i = 0;

    while (str[i])  /* not the null character */
    {
        putchar(str[i]);
        if(++i % 4 == 0 && str[i]) // через каждые четыре символа
            putchar(' '); // вставка пробела
    }
}
