/* count.c -- using standard I/O */
#include <stdio.h>
#include <stdlib.h> // exit() prototype

int main(int argc, char *argv[])
{
    int ch;         // place to store each character as read
    FILE *fp;       // "file pointer"
    unsigned long count = 0;
	
	printf("Can't open %d\n", argc);
	
    if (argc != 2) // включая имя программы в argv[0], т.е. если вызов например такой: count.exe 1.txt// тогда argv[0] это count, а argv[1] это 1.txt, всего 2 параметра
    {
        printf("Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        printf("Can't open %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
	// функция getc(fp) получает символ из открытого файла 
	// Функции getc ()  и putc () работают очень похоже на getchar()  и putchar(). Отличие заключается в том, что этим новым функциям потребуется указать, с каким  файлом работать.
    while ((ch = getc(fp)) != EOF)
    {
        putc(ch,stdout);  // same as putchar(ch); // не путать с puts
        count++;
    }
    fclose(fp);
    printf("File %s has %lu characters\n", argv[1], count);
    
    return 0;
}
