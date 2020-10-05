/* p_and_s.c -- pointers and strings */
#include <stdio.h>
int main(void)
{
    const char * mesg = "Don't be a fool!";
    const char * copy;
    
    copy = mesg;
    printf("%s\n", copy);
    printf("mesg = %s; &mesg = %p; *(&mesg) = %p; *mesg = %c; *mesg[0] = %c; *mesg = %p; *mesg[0] = %p; *mesg[1] = %p; *mesg[12] = %p; value = %p\n", mesg, &mesg, *(&mesg), *mesg, mesg[0], *mesg, mesg[0], mesg[1], mesg[12], mesg);
    printf("copy = %s; &copy = %p; value = %p\n",
           copy, &copy, copy);
     
    return 0;
}
/*
&mesg - получаем адрес указателя, разъименовываем его через *(&mesg) и получаем адрес строки mesg

Don't be a fool!
mesg = Don't be a fool!; &mesg = 000000000022FE28; *(&mesg) = 0000000000404000; *mesg = D; 
*mesg[0] = D; *mesg = 0000000000000044; *mesg[0] = 0000000000000044; *mesg[1] = 000000000000006F; *mesg[12] = 000000000000006F; value = 0000000000404000
copy = Don't be a fool!; &copy = 000000000022FE20; value = 0000000000404000

44 - код символа < D >; 6F - код символа < o >
*/
