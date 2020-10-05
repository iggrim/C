/* films1.c -- using an array of structures */
#include <stdio.h>
#include <string.h>
#define TSIZE        45      /* size of array to hold title   */
#define FMAX         5       /* maximum number of film titles */

struct film {
    char title[TSIZE];
    int rating;
};
char * s_gets(char str[], int lim);
int main(void)
{
	// movies не является именем суруктуры; оно представляет собой имя массива, элементы которого — структуры типа struct film.
    struct film movies[FMAX];
    int i = 0;
    int j;
    
    puts("Enter first movie title:");
    while (i < FMAX && s_gets(movies[i].title, TSIZE) != NULL &&
           movies[i].title[0] != '\0')// здесь movies[i].title[0] это значение, а &movies[i++].rating в scanf, это адрес
    {
        puts("Enter your rating <0-10>:");
		scanf("%d", &movies[i++].rating);//продвигаемся по циклу инкриментируя значение i
        //scanf("%d", &(movies[i++].rating)); // работает //scanf принимает адрес
        // flizny == &flizny[0];  // имя массива является адресом его первого элемента 
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
