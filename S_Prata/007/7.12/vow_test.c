// vowels.c -- uses multiple labels
#include <stdio.h>
int main(void)
{
    char ch;
    int a_ct, e_ct, i_ct, o_ct, u_ct;
    
    a_ct = e_ct = i_ct = o_ct = u_ct = 0;
    
    printf("Enter some text; enter # to quit.\n");
    
	while((ch = getchar()) != '\n')
	     putchar(ch);		

    printf("\n number of vowels:   A    E    I    O    U\n");
    printf("                  %4d %4d %4d %4d %4d\n",
           a_ct, e_ct, i_ct, o_ct, u_ct);
    
    return 0;
}
