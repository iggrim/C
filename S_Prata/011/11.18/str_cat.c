/* str_cat.c -- joins two strings */
#include <stdio.h>
#include <string.h>  /* declares the strcat() function */
#define SIZE 80
char * s_gets(char * st, int n); // функция s_gets возвращает указатель
int main(void)
{
    char flower[SIZE];
    char addon[] = "s smell like old shoes.";
    
    puts("What is your favorite flower?");
    if (s_gets(flower, SIZE))
    {
        strcat(flower, addon);
        puts(flower);
        puts(addon);
    }
    else
        puts("End of file encountered!");
    puts("bye");

    return 0;
}


char * s_gets(char * st, int n) // функция s_gets возвращает указатель
{
    char * ret_val;
    int i = 0;
    
	// Функция fgets () возвращает указатель на char. Если все проходит нормально, она просто возвращает тот же адрес, который был ей передан в первом аргументе. 
    ret_val = fgets(st, n, stdin); // fgets читает из буфера входного потока 80 символов удаляя их из буфера
    if (ret_val)
    {
        while (st[i] != '\n' && st[i] != '\0')
            i++;
        if (st[i] == '\n')
            st[i] = '\0';
        else // must have words[i] == '\0'
            while (getchar() != '\n') // getchar() читает посимвольно то, что осталось в буфере
                continue;
    }
    return ret_val;
}
