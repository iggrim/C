#include <stdio.h>
#include <string.h>      // for strlen() prototype
#include <Windows.h>
#define DENSITY 62.4     // human density in lbs per cu ft
int main()
{
    float weight, volume;
    int size, letters;
    char name[40];        // name is an array of 40 chars
	
	char ch;
	
	// ======файл обязательно должен быть сохранен в кодировке ANSI=====
	SetConsoleCP(1251); // <Windows.h> Функция SetConsoleCP устанавливает кодовую страницу ввода
    SetConsoleOutputCP(1251);// <Windows.h>
    
	printf("Введите символ\n");
    scanf("%c", &ch);   
    printf("Код для  %c - %d.\n", ch, ch);
	
	
    printf("Hi! What's your first name?\n");
    scanf("%s", name);
	
    printf("%s, what's your weight in pounds?\n", name);
    scanf("%f", &weight);
    size = sizeof name;
    letters = strlen(name);
    volume = weight / DENSITY;
    printf("Well, %s, your volume is %2.2f cubic feet.\n",
           name, volume);
    printf("Also, your first name has %d letters,\n",
           letters);
    printf("and we have %d bytes to store it.\n", size);
    
    return 0;
}
