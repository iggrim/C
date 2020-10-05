// sum_arr1.c -- sums the elements of an array
// use %u or %lu if %zd doesn't work
#include <stdio.h>
#define SIZE 10
int sum(int ar[], int n);
int main(void)
{
    int marbles[SIZE] = {20,10,5,39,4,16,19,26,31,20};
    long answer;
    
    answer = sum(marbles, SIZE);
    printf("The total number of marbles is %ld.\n", answer);
    //printf("The size of marbles is %zd bytes.\n", // %zd - не поддерживается
    printf("The size of marbles is %u bytes.\n",
           sizeof marbles);
    
    return 0;
}

//int sum(int ar[], int n)     // how big an array? //Предупреждение
int sum(int *ar, int n)     // how big an array?   
{
    int i;
    int total = 0;
    
    for( i = 0; i < n; i++)
        total += ar[i];
    //printf("The size of ar is %zd bytes.\n", sizeof ar); // %zd - не поддерживается
    printf("The size of ar is %u bytes.\n", sizeof ar); // массив marbles содержит 10 значений типа int, каждое из которых занимает 4 байта, что в сумме составляет 40 байт. Но размер ar равен всего 8 байтов. Причина в том, что ar — это не сам массив, а указатель на первый элемент marbles. В моей системе для хранения адресов применяются 8 байтов, поэтому размером переменной типа указателя будет 8 байтов.
    
    return total;
}
