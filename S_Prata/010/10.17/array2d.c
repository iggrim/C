// array2d.c -- functions for 2d arrays
#include <stdio.h>
#define ROWS 3
#define COLS 4

void sum_rows(int ar[][COLS], int rows); // Пустые квадратные скобки идентифицируют ar в качестве указателя. // ar[][COLS] указатель на массив из четырех значений int, т.е. указатель на одну строку двкмерного мпсиива
void sum_cols(int [][COLS], int ); // пустые квадратные скобки идентифицируют ar в качестве указателя // имена можно опустить // int [][COLS] указатель на массив из четырех значений int 
int sum2d(int (*ar)[COLS], int rows); // another syntax // int (*ar)[COLS] указатель на массив из четырех значений int 
int main(void)
{
    int junk[ROWS][COLS] = {
        {2,4,6,8},
        {3,5,7,9},
        {12,10,8,6}
    };
    
    sum_rows(junk, ROWS);
    sum_cols(junk, ROWS);
    printf("Sum of all elements = %d\n", sum2d(junk, ROWS));
    
    return 0;
}

void sum_rows(int ar[][COLS], int rows)
{
    int r;
    int c;
    int tot;
    
    for (r = 0; r < rows; r++)
    {
        tot = 0;
        for (c = 0; c < COLS; c++)
            tot += ar[r][c];
        printf("row %d: sum = %d\n", r, tot);
    }
}

void sum_cols(int ar[][COLS], int rows)
{
    int r;
    int c;
    int tot;
    
    for (c = 0; c < COLS; c++)
    {
        tot = 0;
        for (r = 0; r < rows; r++)
            tot += ar[r][c];
        printf("col %d: sum = %d\n", c, tot);
    }
}

//int sum2d(int ar[][COLS], int rows) // Пустые квадратные скобки идентифицируют ar в качестве указателя. // или
//int sum2d(int (*ar)[4], int rows) // или
int sum2d(int (*ar)[COLS], int rows)  // подстановка т.к. #define COLS 4
{
    int r;
    int c;
    int tot = 0;
    
    for (r = 0; r < rows; r++)
        for (c = 0; c < COLS; c++) // 
            tot += ar[r][c];
    
    return tot;
}
