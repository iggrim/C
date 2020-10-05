/*  fgets2.c  -- using fgets() and fputs() */
#include <stdio.h>
#define STLEN 10
int main(void)
{
    char words[STLEN];
    
    puts("Enter strings (empty line to quit):");
	//  в данной программе функция fgets () читает STLEN-1, т.е. 9 символов за раз.
    while (fgets(words, STLEN, stdin) != NULL && words[0] != '\n') //words[0] != '\n' нажали Enter
        fputs(words, stdout);
    puts("Done.");
    
    return 0;
}
