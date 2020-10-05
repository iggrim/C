#include <stdio.h>
#include "ext_ind.h" // здесь обязательно, это extern int units;
// #include "ext_func.h" //это лишнее, достаточно просто void critic(void);

// void critic(void); // можно не указывать

void critic(void)
{
    printf("No luck, my friend. Try again.\n");
    scanf("%d", &units);
}