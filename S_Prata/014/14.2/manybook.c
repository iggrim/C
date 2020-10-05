/* manybook.c -- multiple book inventory */
#include <stdio.h>
#include <string.h>
char * s_gets(char * st, int n);
#define MAXTITL   40
#define MAXAUTL   40
#define MAXBKS   100              /* maximum number of books  */

struct book {                     /* set up book template     */
    char title[MAXTITL];
    char author[MAXAUTL];
    float value;
};

int main(void)
{
	// Само имя library не является именем суруктуры; оно представляет со­бой имя массива, элементы которого — структуры типа struct book.
    struct book library[MAXBKS]; /* array of book structures // массив структур типа book*/
    int count = 0;
    int index;
    
    printf("Please enter the book title.\n");
    printf("Press [enter] at the start of a line to stop.\n");
    while (count < MAXBKS && s_gets(library[count].title, MAXTITL) != NULL
           && library[count].title[0] != '\0')
    {
        printf("Now enter the author.\n");
        s_gets(library[count].author, MAXAUTL);
        printf("Now enter the value.\n");
        scanf("%f", &library[count++].value);
        while (getchar() != '\n')
            continue;          /* clear input line         */
        if (count < MAXBKS)
            printf("Enter the next title.\n");
    }
    
    if (count > 0)
    {
        printf("Here is the list of your books:\n");
        for (index = 0; index < count; index++)
            printf("%s by %s: $%.2f\n", library[index].title,
                   library[index].author, library[index].value);
    }
    else
    	printf("No books? Too bad.\n");
    
    return 0;
}

char * s_gets(char * st, int n)
{
    char * ret_val;
    char * find;
    
	// Функция fgets () возвращает указатель на char. Если все проходит нормально, она просто возвращает тот же адрес, который был ей передан в первом аргументе. 
	// если функция fgets встречает конец файла, она возвращает специальный указатель, называемый нулевым. Такой указатель гарантированно не ссылается на реальные данные
    ret_val = fgets(st, n, stdin);
    if (ret_val)
    {
        find = strchr(st, '\n');   // look for newline // если имя больше n (т.е. MAXAUTL или  MAXTITL), то символа '\n' в куске строки нет
        if (find)                  // if the address is not NULL,
            *find = '\0';          // place a null character there
        else
            while (getchar() != '\n') // то что после n символов и до '\n' отбрасываем (удаляем из буфера)
                continue;          // dispose of rest of line
    }
    return ret_val;
}
