// sizeof.c -- uses sizeof operator
// uses C99 %z modifier -- try %u or %lu if you lack %zd
// sizeof.c — использование операции sizeof
// в примере применяется определенный в стандарте с99 модификатор %z; если в вашей 
// системе модификатор %zd не поддерживается, попробуйте вместо него %u или %lu 
#include <stdio.h>
int main(void)
{
    int n = 0;
    size_t intsize;
    
    intsize = sizeof (int);
    //printf("n = %d, n has %zd bytes; all ints have %zd bytes.\n",  n, sizeof n, intsize );
	
    printf("n = %d, n has %u bytes; all ints have %u bytes.\n",
           n, sizeof n, intsize );
    
    return 0;
}
