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
        while (st[i] != '\0')
            i++;
       
            while (getchar() != '\n') // в таком варианте в буфере ничего нет и getchar() в этой точке программы ждет ввод с клавиатыры, т.е. после ввода символов программа продолжится отсюда, что соответственно увеличит кол-во вводимых строк до 20 при выполнении условия ct < LIM (ct < 10)
                continue;
    }
    return ret_val;
}
