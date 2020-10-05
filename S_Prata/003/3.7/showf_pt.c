/* showf_pt.c -- displays float value in two ways */
#include <stdio.h>
int main(void)
{
    float aboat = 32000.0;
    double abet = 2.14e9;
    long double dip = 5.32e-5;
    
    printf("%f can be written %e\n", aboat, aboat);
    // next line requires C99 or later compliance
	// для вывода следующей строки требуется компилятор,
    // поддерживающий С99 или более поздний стандарт
    printf("And it's %a in hexadecimal, powers of 2 notation\n", aboat);
    printf("%f can be written %e\n", abet, abet);
    printf("%Lf can be written %Le\n", dip, dip);
    
    return 0;
}

/* showf_pt.c -- отображает значение типа float двумя способами 
#include <stdio.h> 
int main(void)
(
float aboat = 32000.0; 
doUie abet = 2.14е9; 
long douMe dip = 5.32е-5;

// для вывода как float , так и double используется спецификатор %f, %е или %а.
printf("%f может быть записано как %e\n", aboat, aboat);

// для вывода следующей строки требуется компилятор, поддерживающий С99 или более поздний стандарт
printf ("И его %а в шестнадцатеричной, представляющей степени 2, форме записи\п”, aboat); 

printf("%f может быть записано как %e\n", abet, abet); 

// Для вывода данных типа long double требуются спецификаторы %Lf, %Le и %La. 
printf("%Lf может быть записано как %Le\n", dip, dip);
return О;
)

========================
вывод, при условии, что компилятор совместим со стандартом 
C99/C11
32000.000000 может быть записано как 3.200000е+04
И его Oxl.f4p+l4 в шестнадцатеричной, представляющей степени 2, форме записи
2140000000.000000 может быть записано как 2.140000е+О9
0.000053 может быть записано как 5.320000е-05

*/