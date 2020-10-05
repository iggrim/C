/* addaword.c -- uses fprintf(), fscanf(), and rewind() */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 41

int main(void)
{
    FILE *fp;
    char words[MAX];
    
    if ((fp = fopen("wordy", "a+")) == NULL)
    {
        fprintf(stdout,"Can't open \"wordy\" file.\n");
        exit(EXIT_FAILURE);
    }
    
    puts("Enter words to add to the file; press the #");
    puts("key at the beginning of a line to terminate.");
    while ((fscanf(stdin,"%40s", words) == 1)  && (words[0] != '#')) // char words [MAX]
        fprintf(fp, "%s\n", words);
    
    puts("File contents:");
    rewind(fp);           /* go back to beginning of file */
	
	//функция fscanf(),как и scanf(), возвращает количество прочитанных ею элементов,  поэтому она возвратит 1, если прочитает число или, если указан строковый спецификатор, символы до пробела
    while (fscanf(fp,"%s",words) == 1) // char words [MAX]
        puts(words);
    puts("Done!");
    if (fclose(fp) != 0)
        fprintf(stderr,"Error closing file\n");
    
    return 0;
}
