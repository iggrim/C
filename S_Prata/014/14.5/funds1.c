/* funds1.c -- passing structure members as arguments */
#include <stdio.h>
#define FUNDLEN 50

struct funds {
    char   bank[FUNDLEN];
    double bankfund;
    char   save[FUNDLEN];
    double savefund;
};

double sum(double, double); // При  желании имена переменных ( формальные параметры ) в прототипе можно не указывать: (Гл.9) // Фактический аргумент — это выражение, указанное в круглых скобках при вызове функции. Формальный параметр - это переменная, объявленная в заголовке определения функции. Когда функция вызывается, переменные, объявленные как формальные параметры, создаются и инициализируются значениями, которые получены в результате вычисления фактических аргументов.


int main(void)
{
    struct funds stan = {
        "Garlic-Melon Bank",
        4032.27,
        "Lucky's Savings and Loan",
        8543.94
    };
    
    printf("Stan has a total of $%.2f.\n",
           sum(stan.bankfund, stan.savefund) );
    return 0;
}

/* adds two double numbers */
double sum(double x, double y) // обязательно указать тип формальных параметров 
{
    return(x + y);
}
