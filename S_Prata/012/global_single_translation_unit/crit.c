#include <stdio.h>
#include "critic.h"

void critic(void)
{
	// extern int units; // при такой реализации у нас есть доступ к статической переменной с внешним связыванием лбявленной в другом файле как int units
    printf("No luck, my friend. Try again.\n");
    //scanf("%d", &units);
}