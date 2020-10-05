//  align.c -- using _Alignof and _Alignas  (C11)

#include <stdio.h>
int main(void)
{
    double dx;
    char ca;
    char cx;
    double dz;
    char cb;
	//char  cz;
    char _Alignas(double)  cz;
    
   
   
    printf("char alignment:   %d\n", _Alignof(char));
    printf("double alignment: %d\n", _Alignof(double));
    printf("&dx: %p\n", &dx);
    printf("&ca: %p\n", &ca);
    printf("&cx: %p\n", &cx);
    printf("&dz: %p\n", &dz);
    printf("&cb: %p\n", &cb);
    printf("&cz: %p\n", &cz);
    
    return 0;
}
/*
char alignment:   1
double alignment: 8
&dx: 000000000022FE48
&ca: 000000000022FE47
&cx: 000000000022FE46
&dz: 000000000022FE38
&cb: 000000000022FE37
&cz: 000000000022FE36 ======== // //char  cz;
----------------------
char alignment:   1
double alignment: 8
&dx: 000000000022FE48
&ca: 000000000022FE47
&cx: 000000000022FE46
&dz: 000000000022FE38
&cb: 000000000022FE37
&cz: 000000000022FE30 ========= // char _Alignas(double)  cz;
*/
