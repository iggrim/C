/* binbit.c -- using bit operations to display binary */
#include <stdio.h>
#include <limits.h>  // for CHAR_BIT, # of bits per char
char * itobs(int, char *);
void show_bstr(const char *);

int main(void)
{
    // CHAR_BIT �� ������������� ����� limits.h. //  ������ ������������ ���������� ����� � ���� char
    char bin_str[CHAR_BIT * sizeof(int) + 1]; // sizeof(int) = 4 (����� ) // CHAR_BIT = 8 ���
    // �.�. bin_str{32}
    int number;

    puts("Enter integers and see them in binary.");
    puts("Non-numeric input terminates program.");
    while (scanf("%d", &number) == 1)
    {
        itobs(number,bin_str);
        printf("%d is ", number);
        show_bstr(bin_str);
        putchar('\n');
    }
    puts("Bye!");

    return 0;
}

char * itobs(int n, char * ps)// �������� ����� 25 ��� 11001����
{
    int i;
    const static int size = CHAR_BIT * sizeof(int); // size=32 (8�4=32)

    for (i = size - 1; i >= 0; i--, n >>= 1)// �������� ������ 1101 �� �������, �������� 1100 �.�. 12
        ps[i] = (01 & n) + '0'; // ����������� 01 & n ����� �������� ���������� ���� � n.
        //��������� �������� � ��� 1, �� ��� ������� ��������� ������ '0' ��� ������ '1.
        //�������������� �������������� ����������� ���� ��� '0'

    ps[size] = '\0';

    return ps;
}

/* show binary string in blocks of 4 */
void show_bstr(const char * str)
{ // ������� show_bstr (), ������� ��������� ������������������ ����� �� ������ �� ������, ����� ��������� ���������� ������.
    int i = 0;

    while (str[i])  /* not the null character */
    {
        putchar(str[i]);
        if(++i % 4 == 0 && str[i]) // ����� ������ ������ �������
            putchar(' '); // ������� �������
    }
}
