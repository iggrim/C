/* echo_eof.c -- repeats input to end of file */
#include <stdio.h>
int main(void)
{   
    //char ch; // переменные типа char могут быть представлены целочисленными значениями без знака в диапазоне от О до 255, но EOF может иметь числовое значение -1.
    int ch; // поэтому тип переменной ch изменен с char на int
	
    while ((ch = getchar()) != EOF)
        putchar(ch);
    
    return 0;
}

// выход из программы Ctrl+Z

// Перенаправление ввода или перенаправление stdin // echo_eof.exe < text.txt // файл text.txt ассоциируется с потоком stdin // наверное было бы корректнее назвать это переназначением потока ввода
