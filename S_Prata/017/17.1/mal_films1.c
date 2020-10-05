/* films1.c -- using an array of structures */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TSIZE        45      /* size of array to hold title   */
//#define FMAX         5       /* maximum number of film titles */

struct film {
    char title[TSIZE];
    int rating;
};
char * s_gets(char str[], int lim);

int main(void)
{
	// movies не является именем суруктуры; оно представляет собой имя массива, элементы которого — структуры типа struct film.
    //struct film movies[FMAX];
    struct film * movies;
    int i = 0;
    int n;
    int j;
    
    //puts("Enter first movie title:");
	
	printf("Enter the maximum number of movies you'll enter:\n");
    scanf("%d", &n);
	
	/*	Во время выполнения программы можно выделять дополнительную память. Основным инструментом является функция malloc(), которая принимает один аргумент: нужное количество байтов памяти. Затем malloc() ищет подходящий блок свободной памяти. Память будет анонимной, т.е. функция malloc() выделяет блок памяти, но не назначает ему имя. Тем не менее, она возвращает адрес первого байта в этом блоке. Следовательно,вы можете присвоить этот адрес переменной типа указателя и применять такой указатель для доступа в память.     (Гл. 12)*/
    movies = (struct film *) malloc(n * sizeof(struct film)); // адрес первого байта в блоке
	
	// Макрос getchar() возвращает очередной символ из файла stdin. Символ считывается как переменная типа unsigned char, преобразованная к целому.
	while(getchar() != '\n') // в данном случае удаляем из буфера нулевой символ  конца строки
            continue; /**/
			
	puts("Enter first movie title:");
    //while (i < FMAX && s_gets(movies[i].title, TSIZE) != NULL &&
    while (i < n && s_gets(movies[i].title, TSIZE) != NULL &&
           movies[i].title[0] != '\0') // здесь movies[i].title[0] это значение, а &movies[i++].rating в scanf, это адрес
    {
        puts("Enter your rating <0-10>:");
        scanf("%d", &movies[i++].rating); // продвигаемся по циклу инкриментируя значение i // flizny == &flizny[0];  // имя массива является адресом его первого элемента
        while(getchar() != '\n')
            continue;
        puts("Enter next movie title (empty line to stop):");
    }
	
    if (i == 0)
        printf("No data entered. ");
    else
        printf ("Here is the movie list:\n");
    
    for (j = 0; j < i; j++)
        printf("Movie: %s  Rating: %d\n", movies[j].title,
               movies[j].rating);
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
