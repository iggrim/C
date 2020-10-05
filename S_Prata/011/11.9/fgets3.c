/*  fgets3.c  -- using fgets() */
#include <stdio.h>
#define STLEN 10
int main(void)
{
    char words[STLEN];
    int i;
	char ch;
    
    puts("Enter strings (empty line to quit):");
	// fgets(words, STLEN, stdin) != NULL это проверка на пучтую строку(для завершения программы)
    while (fgets(words, STLEN, stdin) != NULL && words[0] != '\n')
    {
        i = 0;
        while (words[i] != '\n' && words[i] != '\0')
            i++;
        if (words[i] == '\n')
            words[i] = '\0';
        else // must have words[i] == '\0'
		    // while (getchar()!= '\n') // было
            while ((ch = getchar()) != '\n')// Функция getchar () не принимает аргументов и возвращает очередной символ из входного потока.
            {    
			   continue; // было
			   // putchar(ch); 
			}
        puts(words);        
    }
    puts("done");
    return 0;
}
/*
после применения putchar(ch); 
1234567890qwe
0qwe123456789

fgets читает из буфера входного потока 10 символов удаляя их из буфера
getchar() читает посимвольно то, что осталось в буфере
*/
