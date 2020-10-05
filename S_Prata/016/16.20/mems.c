// mems.c -- using memcpy() and memmove()
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 10
void show_array(const int ar[], int n);
// remove following if C11 _Static_assert not supported
_Static_assert(sizeof(double) == 2 * sizeof(int), "double not twice int size");
int main()
{
    int values[SIZE] = {1,2,3,4,5,6,7,8,9,10};
    int target[SIZE];
    double curious[SIZE / 2] = {2.0, 2.0e5, 2.0e10, 2.0e20, 5.0e30};
    
    puts("memcpy() used:");
    puts("values (original data): ");
    show_array(values, SIZE);
    memcpy(target, values, SIZE * sizeof(int));
    puts("target (copy of values):");
    show_array(target, SIZE);
    
    puts("\nUsing memmove() with overlapping ranges:");
    memmove(values + 2, values, 5 * sizeof(int));
    puts("values -- elements 0-5 copied to 2-7:");
    show_array(values, SIZE);
    
    puts("\nUsing memcpy() to copy double to int:");
    memcpy(target, curious, (SIZE / 2) * sizeof(double));
    puts("target -- 5 doubles into 10 int positions:");
    show_array(target, SIZE/2);
    show_array(target + 5, SIZE/2);
   
    return 0;
}

void show_array(const int ar[], int n)
{
    int i;
    
    for (i = 0; i < n; i++)
        printf("%d ", ar[i]);
    putchar('\n');
}


/*
Использование memcpy(): 
значения (исходные данные):
123456789  10
целевые данные (копия значений):
123456789  10
Использование memmove() с перекрывающимися областями: 
значения -- элементы 0-5 скопированы в элементы 2-7:
1 2 1 2 3 4 5 8 9 10
Использование memcpy () для копирования double в int: 
целевые данные -- 5 значений double в 10 позициях int:
0 1073741824 0 1091070464 536870912
1108516959 2025163840 1143320349 -2012696540 1179618799
*/
