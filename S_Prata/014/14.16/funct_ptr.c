// func_ptr.c -- uses function pointers
#include <stdio.h>
#include <string.h>
#include <ctype.h> // Семейство функций анализа символов, предоставляемых заголовочным файлом ctype.h Стандартный набор функций для анализа символов; их прототипы содержатся в заголовочном файле ctype. h.
#define LEN 81
char * s_gets(char * st, int n);
char showmenu(void);
void eatline(void);     // read through end of line
void show(void (* fp)(char *), char * str); //(* fp)- указатель на функцию возвращающую тип void
void ToUpper(char *);   // convert string to uppercase
void ToLower(char *);   // convert string to uppercase
void Transpose(char *); // transpose cases
void Dummy(char *);     // leave string unaltered
//void Exi();

int main(void)
{
    char line[LEN];
    char copy[LEN];
    char choice;
	
	// указывает на функцию, которая имеет аргумент типа char * и ничего не возвращает
    void (*pfun)(char *); // points a function having a
    // char * argument and no return value
	
    puts("Enter a string (empty line to quit):");
    while (s_gets(line, LEN) != NULL && line[0] != '\0')
    {
        while ((choice = showmenu()) != 'n')
        {
            switch (choice   )  // switch sets pointer
            {
                case 'u' : pfun = ToUpper;   break; // ToUpper - адрес функции 
                case 'l' : pfun = ToLower;   break; // ToLower - адрес функции 
                case 't' : pfun = Transpose; break; // -----//-----
                case 'o' : pfun = Dummy;     break; // -----//-----
				//case 'q' : pfun = Exi;
				case 'q' : break;
            }
			if(choice == 'q')
			 goto exi;
		 
            strcpy(copy, line);// make copy for show() // line - введенная строка
            show(pfun, copy);  // use selected function
        }
        puts("Enter a string (empty line to quit):");
    }
	
	exi:
    puts("Bye!");
    return 0;
}

char showmenu(void)
{
    char ans;
	//============================ вывод на экран иеню
    puts("Enter menu choice:");
    puts("u) uppercase       l) lowercase");
    puts("t) transposed case o) original case");
    puts("n) next string");
    puts("q) exit");
	
	//================================================
    ans = getchar();    // get response
    ans = tolower(ans); // convert to lowercase
    eatline();          // dispose of rest of line
	
	// char *strchr (const char * s, int с) ; Эта функция возвращает указатель на первую ячейку строки s, в которой содержится символ с. (Завершающий нулевой символ является частью строки, так что его тоже можно искать.) Если символ не найден, функция возвращает нулевой указатель.
    //while (strchr("ulton", ans) == NULL)
    while (strchr("ultonq", ans) == NULL)
    {   // если введенные данные не соответствуют меню
        puts("Please enter a u, l, t, o, or n:");
        ans = tolower(getchar());
        eatline();
    }
    
    return ans;
}

void eatline(void)
{
    while (getchar() != '\n')
        continue;
}

/* void Exi(void)
{
	return 0;
} */

void ToUpper(char * str)
{
    while (*str)
    {
        *str = toupper(*str);
        str++;
    }
}

void ToLower(char * str)
{
    while (*str)
    {
        *str = tolower(*str);
        str++;
    }
}
void Transpose(char * str)
{
    while (*str)
    {
        if (islower(*str))
            *str = toupper(*str);
        else if (isupper(*str))
            *str = tolower(*str);
        str++;
    }
}

void Dummy(char * str)
{
    // leaves string unchanged
}

void show(void (* fp)(char *), char * str)
{
    (*fp)(str); // apply chosen function to str
    puts(str);  // display result
	
}

char * s_gets(char * st, int n)
{
    char * ret_val;
    char * find;
    
    ret_val = fgets(st, n, stdin);
    if (ret_val)
    {
        find = strchr(st, '\n');   // look for newline
        if (find)                  // if the address is not NULL,
            *find = '\0';          // place a null character there
        else
            while (getchar() != '\n')
                continue;          // dispose of rest of line
    }
    return ret_val;
}
