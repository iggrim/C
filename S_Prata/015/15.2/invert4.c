/* invert4.c -- using bit operations to display binary */
#include <stdio.h>
#include <limits.h>
char * itobs(int, char *);
void show_bstr(const char *);
int invert_end(int num, int bits);

int main(void)
{
    char bin_str[CHAR_BIT * sizeof(int) + 1];// 8*4=32// Массив bin_str содержит на один элемент больше величины 32, чтобы можно было добавить в него завершающий нулевой символ.
    
    int number;
    
    puts("Enter integers and see them in binary.");
    puts("Non-numeric input terminates program.");
    while (scanf("%d", &number) == 1)
    {
        itobs(number,bin_str);
        printf("%d is\n", number);
        show_bstr(bin_str);
        putchar('\n');
        number = invert_end(number, 4);
        printf("Inverting the last 4 bits gives\n");
        show_bstr(itobs(number,bin_str));
        putchar('\n');
    }
    puts("Bye!");
    
    return 0;
}

char * itobs(int n, char * ps) // например ввели ===25=== это 11001 
{
    int i;
    const static int size = CHAR_BIT * sizeof(int);
    
	// формируем массив из нулей и едениц - эквивалент введенного десятичного значения
    for (i = size - 1; i >= 0; i--, n >>= 1) // сдвигаем вправо 11001 на еденицу, получаем 1100 т.е. 12 
        ps[i] = (01 & n) + '0'; // результатом 01 & n будет значение последнего бита в n.
        //Значением является 0 или 1, но для массива необходим символ '0' или символ '1.
        //Преобразование осуществляется добавлением кода для '0'
    ps[size] = '\0'; // ps[32]='\0' - это тридцать третий элемент
    
    return ps;
}

/* show binary string in blocks of 4 */
void show_bstr(const char * str)
{
    int i = 0;
	
    while (str[i])  /* not the null character */
    {
        putchar(str[i]);
        if(++i % 4 == 0 && str[i])
            putchar(' ');
    }
}

int invert_end(int num, int bits) // bits=4 
{
    int mask = 0;
    int bitval = 1;
    
    while (bits-- > 0) // пока bits больше нуля 
    {
        mask |= bitval; // формируем маску 1111 (или 15)
        bitval <<= 1; // сдвигаем bitval изначально равный еденице в право на позицию 2-ки, затем 4-ки, 8, 16
    }
    
    return num ^ mask;
}
