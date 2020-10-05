// boolean.c -- using a _Bool variable
#include <stdio.h>
#include <stdbool.h>
int main(void)
{
    long num;
    long sum = 0L;
    bool input_is_good; // подключил stdbool.h и заменил _Bool на bool
    
    printf("Please enter an integer to be summed ");
    printf("(q to quit): ");
    input_is_good = (scanf("%ld", &num) == 1);
    while (input_is_good)
    {
        sum = sum + num;
        printf("Please enter next integer (q to quit): "); //  любой символ, не число
        input_is_good = (scanf("%ld", &num) == 1);
    }
    printf("Those integers sum to %ld.\n", sum);
    
    return 0;
}
