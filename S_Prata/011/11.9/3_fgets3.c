/*  fgets3.c  -- using fgets() */
#include <stdio.h>
#define STLEN 10
int main(void)
{
    char words[STLEN];
    int i;
	char ch;
    
    puts("Enter strings (empty line to quit):");
    fgets(words, STLEN, stdin);
    
    puts(words);        
   
    puts("done");
    return 0;
}
/*

fgets читает из буфера входного потока 10 символов удаляя их из буфера
getchar() читает посимвольно то, что осталось в буфере
*/
