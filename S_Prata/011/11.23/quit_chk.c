/* quit_chk.c -- beginning of some program */
#include <stdio.h>
#include <string.h>
#define SIZE 80
#define LIM 10
#define STOP "quit"
char * s_gets(char * st, int n);

int main(void)
{
    char input[LIM][SIZE]; // 10 строк по 80 символов максимум
    int ct = 0;
    
    printf("Enter up to %d lines (type quit to quit):\n", LIM);
    while (ct < LIM && s_gets(input[ct], SIZE) != NULL &&  strcmp(input[ct],STOP) != 0)
    {
        ct++;
    }
    printf("%d strings entered\n", ct);
    
    return 0;
}

char * s_gets(char * st, int n)
{
    char * ret_val;
    int i = 0;
    
	//  функция fgets() считывает буфер блоками символов (размер блока определяет второй параметр) вставляя после блока /0, а после нажатия клавиши Enter сохраняет конец строки как \n \0,  
    ret_val = fgets(st, n, stdin); //  читаем сразу по n символов т.е. по 80
    if (ret_val)
    {
        while (st[i] != '\n' && st[i] != '\0')
            i++;
        if (st[i] == '\n')
            st[i] = '\0';
        else // must have words[i] == '\0'
            while (getchar() != '\n')
                continue;
    }
    return ret_val;
}
/*
если здесь строка длинее 80 символов, то st[i] будет = '\0') и строка будет обрезана, если строка менее 80 символов и нажат Enter или длинный текст в консоли переносится системой на новую строку, то st[i] будет = '\n' который будет заменен на символ /0, тогда у нас будет прочитана только одна строка содержащая 80 символов, остальные символы будут отброшены,  в обоих случаях цикл while завершается
*/
