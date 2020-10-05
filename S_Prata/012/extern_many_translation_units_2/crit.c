#include <stdio.h>
//#include "ext_ind.h"
//#include "ext_func.h"

// void critic(void); // можно не указывать

void critic(void)
{
	extern int units; // РАБОТАЕТ
    printf("No luck, my friend. Try again.\n");
    scanf("%d", &units);
}