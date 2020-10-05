//  where.c  -- where's the memory?

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int static_store = 30;
const char * pcg = "String Literal";
int main()
{
    int auto_store = 40;
    char auto_string[] = "Auto char Array";
    int * pi;
    char * pcl;
    
    pi = (int *) malloc(sizeof(int));
    *pi = 35; // динамически выделенные данные
	
    pcl = (char *) malloc(strlen("Dynamic String") + 1); // динамически выделенные данные // выделяем память под строку, плюс нулевой символ
    strcpy(pcl, "Dynamic String"); // strcpy () или strncpy (), делает копию строки, т.е. создает её дубликат // ее типом является char *. Она возвращает значение своего первого аргумента - адрес символа. // первый аргумент не обязательно должен указывать на начало массива; это позволяет копировать только часть массива.
    
    printf("static_store: %d at %p\n", static_store, &static_store);
    printf("  auto_store: %d at %p\n", auto_store, &auto_store);
	
    printf("         *pi: %d at %p\n", *pi, pi); // динамически выделенные данные
	
    printf("  %s at %p\n", pcg, pcg);
    printf(" %s at %p\n", auto_string, auto_string);
	
    printf("  %s at %p\n", pcl, pcl); // динамически выделенные данные
	
    printf("   %s at %p\n", "Quoted String", "Quoted String");
    free(pi);
    free(pcl);
    
    return 0;
}
/*
static_store: 30 at 0000000000403010 // статические данные, включая строковые литералы

  auto_store: 40 at 000000000022FE3C
         *pi: 35 at 00000000004FFFB0
		 
  String Literal at 0000000000404000 // статические данные, включая строковые литералы
  
 Auto char Array at 000000000022FE20
  Dynamic String at 00000000004FE570
  
   Quoted String at 000000000040406E  // статические данные, включая строковые литералы
*/



