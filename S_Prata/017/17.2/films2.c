/* films2.c -- using a linked list of structures // спользование связного списка структур */
#include <stdio.h>
#include <stdlib.h>      /* has the malloc prototype      */
#include <string.h>      /* has the strcpy prototype      */
#define TSIZE    45      /* size of array to hold title   */

struct film {
    char title[TSIZE];
    int rating;
    struct film * next;  /* points to next struct in list */
};
char * s_gets(char * st, int n);
int main(void)
{
    struct film * head = NULL;
    struct film * prev, * current;
    char input[TSIZE];
    
    /* Gather  and store information          */
    puts("Enter first movie title:");
    while (s_gets(input, TSIZE) != NULL && input[0] != '\0') // input это char * ret_val = fgets(st, n, stdin)из функции s_gets
    {
		/*	Во время выполнения программы можно выделять дополнительную память. Основным инструментом является функция malloc(), которая принимает один аргумент: нужное количество байтов памяти. Затем malloc() ищет подходящий блок свободной памяти. Память будет анонимной, т.е. функция malloc() выделяет блок памяти, но не назначает ему имя. Тем не менее, она возвращает адрес первого байта в этом блоке. Следовательно,вы можете присвоить этот адрес переменной типа указателя и применять такой указатель для доступа в память.     (Гл. 12)*/
        current = (struct film *) malloc(sizeof(struct film));
        if (head == NULL)       /* first structure       */
            head = current;
        else                    /* subsequent structures // последующие структуры*/
            prev->next = current; // это понадобится для отображения списка
			
        current->next = NULL;
		
		// Если  необходима копия строки, т.е. ее дубликат, то можно воспользоваться функцией strcpy() или strncpy(). Функция strcpy() предстааляет собой строковый эквивалент оператора присваивания.  strcpy() обладает еще двумя свойствами, которые вы можете счесть удобными. Во-первых, ее типом является char *. Она возвращает значение своего первого аргумента - адрес символа. Во-вторых, первый аргумент не обязательно должен указывать на начало массива; это позволяет копировать только часть массива.
        strcpy(current->title, input);
        puts("Enter your rating <0-10>:");
        scanf("%d", &current->rating);
        while(getchar() != '\n')
            continue;
        puts("Enter next movie title (empty line to stop):");
        prev = current;
    }
    
    /* Show list of movies */ /* Отображение списка фильмов */ 
    if (head == NULL)
        printf("No data entered. ");
    else
        printf ("Here is the movie list:\n");
    current = head;
    while (current != NULL)
    {
        printf("Movie: %s  Rating: %d\n",
               current->title, current->rating);
        current = current->next;
    }
    
    /* Program done, so free allocated memory */
    current = head;
    while (current != NULL)
    {
        free(current);
        current = current->next;
    }
    printf("Bye!\n");
    
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
