// cypher1.c -- alters input, preserving spaces
#include <stdio.h>
#define SPACE ' '             // that's quote-space-quote
int main(void)
{
    char ch;

    //ch = getchar();           // read a character
    //������ ������, �������� Enter, ����������� ����
    while ((ch = getchar()) != 'p')        // while not end of line
    {

        putchar(ch);  // ������ ��� ��������� ������, ������ ��� - \n (��� 10) ��� �������� � �������� �� ����� ������
       //continue;
        //ch = getchar();       // get next character
    }
    putchar(ch);              // print the newline

    return 0;
}
