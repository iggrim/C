// cypher1.c -- alters input, preserving spaces
#include <stdio.h>
#define SPACE ' '             // that's quote-space-quote
int main(void)
{
    char ch;

    //ch = getchar();           // read a character
    //вводим символ, нажимаем Enter, запускается цикл
    while ((ch = getchar()) != 'p')        // while not end of line
    {

        putchar(ch);  // первый раз выводится символ, второй раз - \n (код 10) что приводит к перехлду на новую строку
       //continue;
        //ch = getchar();       // get next character
    }
    putchar(ch);              // print the newline

    return 0;
}
