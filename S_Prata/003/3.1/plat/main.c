/* platinum.c  -- your weight in platinum */
#include <stdio.h> // стандартный заголовочный файл ввода-вывода) заголовочный файл стандартной библиотеки языка Си, содержащий определения макросов, константы и объявления функций и типов, используемых для различных операций стандартного ввода и вывода
int main(void)
{
    float weight;    /* user weight             */
    float value;     /* platinum equivalent     */

    printf("Are you worth your weight in platinum?\n");
    printf("Let's check it out.\n");
    printf("Please enter your weight in pounds: ");

    /* get input from the user                     */
    scanf("%f", &weight);
    /* assume platinum is $1700 per ounce          */
    /* 14.5833 converts pounds avd. to ounces troy */
    value = 1700.0 * weight * 14.5833;
    printf("Your weight in platinum is worth $%.2f.\n", value);
    printf("You are easily worth that! If platinum prices drop,\n");
    printf("eat more to maintain your value.\n");

    getchar();
    getchar();

    return 0;
}
/* Все переменные, используемые для приема значений с помощью функции scanf(), должны отыс­киваться по их адресам. Это значит,
что все аргументы функции должны быть указателями на переменные. Таким образом, С создает возможность передачи по ссылке, и
это позволяет функ­ции изменять содержимое аргумента. Например, чтобы считать целое число и присвоить его значение переменной
count, необходи­мо воспользоваться следующим обращением к scanf(): scanf("%d", &count);
Строки считываются в массивы символов, и имя массива, без всякого указателя, является адре­сом первого элемента массива.
Поэтому, чтобы считать строку в массив символов address, можно использовать команду scanf("%s", address);
В этом случае имя address уже является указателем и не нуждается в префиксе &.*/
