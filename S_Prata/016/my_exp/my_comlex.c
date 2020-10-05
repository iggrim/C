// изыскания
#include <stdio.h>
//#include <math.h>
#include <tgmath.h> // подключает math.h и complex.h плюс добавляет дополнительные возможности по работе с математическими вычислениями

int main(void)
{   
  float x = 44.0; 
  double y;
  y = sqrt(x);  // вызов макроса, следовательно sqrtf(x)
  printf("%.17Lf\n", y);   
  y = (sqrt)(x);  // вызов функции sqrt ()	
    
  printf("%.17Lf\n", y);  // 
    
    
    return 0;
}