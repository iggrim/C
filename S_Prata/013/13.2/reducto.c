// reducto.c -- reduces your files by two-thirds!
#include <stdio.h>
#include <stdlib.h>    // for exit()
#include <string.h>    // for strcpy(), strcat()
#define LEN 40

int main(int argc, char *argv[])
{
    FILE  *in, *out;   // declare two FILE pointers
    int ch;
    char name[LEN];    // storage for output filename
    int count = 0;
    
    // check for command-line arguments
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // set up input
    if ((in = fopen(argv[1], "r")) == NULL)
    {
        fprintf(stderr, "I couldn't open the file \"%s\"\n",
                argv[1]);
        exit(EXIT_FAILURE);
    }
    // set up output
	// strncpy (target, source, n) копирует вплоть до n символов либо до появления нулевого символа (в зависимости от того, что произойдет раньше) из source в target. Следовательно, ====если количество символов в source меньше п====, копируется вся строка, включая нуллевой символ. Эта функция никогда не копирует более n символов, так что если данный лимит исчерпан до достижения конца исходной строки, то нулевой символ не добавляется. 
    strncpy(name,argv[1], LEN - 5); // copy filename
    name[LEN - 5] = '\0'; // это если имя файла содержит 40 символам или больше
    strcat(name,".red");            // append .red
    if ((out = fopen(name, "w")) == NULL)
    {                       // open file for writing
        fprintf(stderr,"Can't create output file.\n");
        exit(3);
    }
    // copy data
	// функция getc(fp) получает символ из открытого файла 
	// Функции getc ()  и putc () работают очень похоже на getchar()  и putchar(). Отличие заключается в том, что этим новым функциям потребуется указать, с каким  файлом работать.
    while ((ch = getc(in)) != EOF)
        if (count++ % 3 == 0)
            putc(ch, out);  // print every 3rd char
    // clean up
    if (fclose(in) != 0 || fclose(out) != 0)
        fprintf(stderr,"Error in closing files\n");
    
    return 0;
}
