/* films3.c -- using an ADT-style linked list */
/* ==========   compile with list.c                        */
#include <stdio.h>
#include <stdlib.h>    /* prototype for exit() */
#include <string.h> // Для strchr
#include "list.h"      /* defines List, Item   */

void showmovies(Item item);
char * s_gets(char * st, int n);
int main(void)
{
    List movies; //Node * List указатель на структуру Node (это title, rating, node *), значит и movies тоже указатель? // устанавливает movies как указатель, подходящий для ссылки на связный список(на первый элемент).
    Item temp; // temp - структура для временного хранения(по идее после добавления записи, т.е. создания очередной структуры, temp надо обнулять)
    
/* == list.h
struct film
{
    char title[TSIZE];
    int rating;
};
typedef struct film Item;
typedef struct node
{
    Item item;
    struct node * next;
} Node;

typedef Node * List;
=== */
    
    /*  инициализация списка пустым содержимым;       */
	// &movies - адрес указателя на заголовок 
    InitializeList(&movies); //&movies(адрес первой структуры типа Node) // разыменованый * plist устанавливаем в NULL;

	// InitializeList(movies); // не приемлемо, не работает // warning: 'movies' используется без инициализации в этой функции т.к. в С аргументы передаются по значению, т.е. будет создана копия указателя с таким же значением, но с другим адресом. Единственный способ позволить ==функции С== изменять значения из ==вызывающей программы== предусматривает применение указателя на эту переменную. 
	// movies = NULL; // приемлемо, работает, вместо InitializeList(&movies)
	
	if (ListIsFull(&movies))
    {
        fprintf(stderr,"No memory available! Bye!\n");
        exit(1);
    }
    
    /* gather and store */
    puts("Enter first movie title:");
    while (s_gets(temp.title, TSIZE) != NULL && temp.title[0] != '\0')
    {
        puts("Enter your rating <0-10>:");
        scanf("%d", &temp.rating);
        while(getchar() != '\n')
            continue;
		// Addltem () вызывается с ==адресом== указателя на заголовок(т.е. на первый элемент в связанном списке) во втором аргументе
        if (AddItem(temp, &movies)==false) // &movies - адрес указателя на заголовок
        {
            fprintf(stderr,"Problem allocating memory\n");
            break;
        }
        if (ListIsFull(&movies))
        {
            puts("The list is now full.");
            break;
        }
		
		// здесь можно обнулит значения временной структуры temp
		
        puts("Enter next movie title (empty line to stop):");
    }
    
    /* display          */
    if (ListIsEmpty(&movies))
        printf("No data entered. ");
    else
    {
        printf ("Here is the movie list:\n");
        Traverse(&movies, showmovies); // &movies это указатель типа List - (List *) 0x22fe48
		// showmovies - указатель на функцию
    }
    printf("You entered %d movies.\n", ListItemCount(&movies));
    
    
    /* clean up         */
    EmptyTheList(&movies); // функция  EmptyTheList  () освобождает память, которая ранее была выделена с помощью malloc():
    printf("Bye!\n");
    
    return 0;
}

void showmovies(Item item)
{
    printf("Movie: %s  Rating: %d\n", item.title,
           item.rating);
}

char * s_gets(char * st, int n)
{
    char * ret_val;
    char * find;
    
	// Функция fgets() считывает до num-1 символов из файла stream и помещает их в массив символов, на который указывает str // В случае удачи fgets() возвращает str, при неудаче возвращается NULL
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
