/* booksave.c -- saves structure contents in a file */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXTITL  40
#define MAXAUTL  40
#define MAXBKS   10             /* maximum number of books */
char * s_gets(char * st, int n);
struct book {                   /* set up book template    */
    char title[MAXTITL];
    char author[MAXAUTL];
    float value;
};

int main(void)
{
    struct book library[MAXBKS]; /* array of structures     */
    int count = 0;
    int index, filecount;
    FILE * pbooks;
    int size = sizeof (struct book);
    
    if ((pbooks = fopen("book.dat", "a+b")) == NULL)
    {
        fputs("Can't open book.dat file\n",stderr);
        exit(1);
    }
    
    rewind(pbooks);            /* go to start of file     */
	// икл while читает одну структуру за раз в массив структур, останааливаясь при заполнении этого массива либо при исчерпании данных в файле.
    while (count < MAXBKS &&  fread(&library[count], size, 1, pbooks) == 1) // читаем файйл по одному блоку, размер блока равен размеру структыры в массиве структур
    {
        if (count == 0)
            puts("Current contents of book.dat:");
        
		printf("%s by %s: $%.2f\n",library[count].title, library[count].author, library[count].value);
        count++;
    }
    filecount = count;
    if (count == MAXBKS)
    {
        fputs("The book.dat file is full.", stderr);
        exit(2);
    }
    
    puts("Please add new book titles.");
    puts("Press [enter] at the start of a line to stop.");
    while (count < MAXBKS && s_gets(library[count].title, MAXTITL) != NULL
           && library[count].title[0] != '\0')
    {
        puts("Now enter the author.");
        s_gets(library[count].author, MAXAUTL);
        puts("Now enter the value.");
        scanf("%f", &library[count++].value); // увеличиваем count с количества имеющихся книг, а filecount остается прежним // самописную функцию s_gets для числа не вызываем
        while (getchar() != '\n')
            continue;                /* clear input line  */
        if (count < MAXBKS)
            puts("Enter the next title.");
    }
    
    if (count > 0)
    {
        puts("Here is the list of your books:");
        for (index = 0; index < count; index++)
            printf("%s by %s: $%.2f\n",library[index].title,
                   library[index].author, library[index].value);
		
		// Мы могли бы воспользоваться циклом и добавлять структуры в конец файла по одной за раз. Однако мы решили прибегнуть к способности функции fwrite() записывать более одного блока за раз. Выражение count - filecount дает количество добавленных новых книг, а вызов fwrite() записывает в файл такое количество блоков размером со структуру.
        fwrite(&library[filecount], size, count - filecount, pbooks); 
		// Выражение count - filecount дает количество добавленных новых книг
		// Выражение &library[filecount] — это адрес первой новой структуры в массиве, поэтому копирование начинается с этой точки (стуктура распологается в ячейках памяти  последовательно).
    }
    else
    	puts("No books? Too bad.\n");
    
    puts("Bye.\n");
    fclose(pbooks);
    
    return 0;
}

char * s_gets(char * st, int n)
{
    char * ret_val;
    char * find;
    
    ret_val = fgets(st, n, stdin);
    if (ret_val)
    {
        find = strchr(st, '\n');   // look for newline
        if (find)                  // if the address is not NULL,
            *find = '\0';          // place a null character there
        else
            while (getchar() != '\n')
                continue;          // dispose of rest of line
    }
    return ret_val;
}

