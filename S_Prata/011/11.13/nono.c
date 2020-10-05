/* nono.c -- no! */
#include <stdio.h>
int main(void)
{
    char side_a[] = "Side A";
    char dont[] = {'W', 'O', 'W', '!' };
    char side_b[] = "Side B";
    
    puts(dont);   /* dont is not a string */ 
    
    return 0;
}
// WOW!Side A // в массиве dont отсутствует завершающий нулевой символ, он не является строкой, поэтому функция puts () не знает, где остановиться. О н а будет выводить содержимое ячеек памяти, следующих за dont, пока не обнаружит нулевой символ где-то в другом месте.