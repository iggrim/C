// pnt_add.c -- pointer addition
#include <stdio.h>
#define SIZE 4
int main(void)
{
    short dates [SIZE];
    short * pti;
    short index;
    double bills[SIZE];
    double * ptf;
    
    pti = dates;    // assign address of array to pointer
    ptf = bills;
    printf("%23s %15s\n", "short", "double");
    for (index = 0; index < SIZE; index ++)
        printf("pointers + %d: %10p %10p\n", index, pti + index, ptf + index);
	
	printf("%10p %10p\n", dates + 2, &dates[2]);
    
    return 0;
}
// В книге опечатка, как в русской версии, так и в английской: *dates + 2 ==  &date[2], надо: dates + 2, &dates[2]

/*
                  short          double
pointers + 0: 000000000022FE30 000000000022FE10
pointers + 1: 000000000022FE32 000000000022FE18
pointers + 2: 000000000022FE34 000000000022FE20
pointers + 3: 000000000022FE36 000000000022FE28
000000000022FE34 000000000022FE34
*/
