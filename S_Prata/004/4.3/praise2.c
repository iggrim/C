/* praise2.c */
// try the %u or %lu specifiers if your implementation
// does not recognize the %zd specifier
#include <stdio.h>
#include <string.h>      /* provides strlen() prototype */
#define PRAISE "You are an extraordinary being."
int main(void)
{
    char name[10];
    
    printf("What's your name? ");
    scanf("%s", name); // функция scanf сама добавляет нулевой символ
    printf("Hello, %s. %s\n", name, PRAISE);
    printf("Your name of %d letters occupies %d memory cells.\n",
           strlen(name), sizeof name); // присутствие нулевого символа сообщает функции strlen (), когда она должна остановить подсчет.
    printf("The phrase of praise has %d letters ",
           strlen(PRAISE));
    printf("and occupies %d memory cells.\n", sizeof PRAISE);
	fputs(name, stdout);
    
    return 0;
}
