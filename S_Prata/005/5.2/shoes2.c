/* shoes2.c -- calculates foot lengths for several sizes */
#include <stdio.h>
#define ADJUST 7.31              // one kind of symbolic constant
int main(void)
{
    const double SCALE = 0.333;  // another kind of symbolic constant
    double shoe, foot;
    
    printf("Shoe size (men's)    foot length\n");
    shoe = 3.0;
    while (shoe < 18.5)      /* starting the while loop */
    {                        /* start of block          */
        foot = SCALE * shoe + ADJUST;
        printf("%10.1f %15.2f inches\n", shoe, foot);
        shoe = shoe + 1.0;
    }                        /* end of block            */
    printf("If the shoe fits, wear it.\n");
    
    return 0;
}
/*
рахмер обуви  (мужской)           длина  ступниув 
=====10=======3.0=========15============8.31  дюймов  // %10.1f %15.2f
              4.0                       8.64  дюймов 
              5.0                       8.97  дюймов 
              6.0                       9.31  дюймов 

             16.0                       12.64  дюймов 
             17.0                       12.97  дюймов 
             18.О                       13.30  дюймов 
Если обувь подходит, носите ее. 
*/
