/* append.c -- appends files to a file // добавление содержимого файлов в файл назначения*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 4096
#define SLEN 81
void append(FILE *source, FILE *dest);
char * s_gets(char * st, int n);

int main(void)
{
    FILE *fa, *fs;	// fa for append file, fs for source file
    int files = 0;  // number of files appended
    char file_app[SLEN];  // name of append file
    char file_src[SLEN];  // name of source file
    int ch;
    
    puts("Enter name of destination file:");
    s_gets(file_app, SLEN); // file_app - адрес (типа char *), где должны сохраняться введенные данные
    if ((fa = fopen(file_app, "a+")) == NULL)
    {
        fprintf(stderr, "Can't open %s\n", file_app);
        exit(EXIT_FAILURE);
    }
	
	// int setvbuf(FILE * restrict fp, char * restrict buf, int mode, size_t size);  // Функция setvbuf () устанавливает альтернативный буфер, предназначенный для применения стандартными функциями ввода-вывода. Указатель fp идентифицирует поток, а buf указывает на используемое хранилище(под буфер). Значение buf, не равное NULL, говорит о том, что буфер вы создаете самостоятельно. Если в качестве значения buf указывается NULL, то функция сама выделит память под буфер. Для mode доступны следующие варианты: _ IOFBF означает полиную буферизац и ю (буфер сбрасывается, когда полон), _IOLBF — построчную буферизацию (буфер сбрасывается, когда полон или когда в него записан символ новой строки) и _IONBF - отсутствие буферизации. Функция возвращает ноль при успешном завершении и ненулевое значение в противном случае.
     // if (setvbuf(fa, NULL, _IOFBF, BUFSIZE) != 0)
    if (setvbuf(fa, NULL, _IOFBF, 5) != 0) // 
    {
        fputs("Can't create output buffer\n", stderr);
        exit(EXIT_FAILURE);
    } /*без setvbuf тоже все сработало*/
	
	
    puts("Enter name of first source file (empty line to quit):"); // ВведИте имя первого исходного файла (или пустую строку для завершения) // далее в цикле while работает s_gets которая обрабатывает входящий поток
    while (s_gets(file_src, SLEN) && file_src[0] != '\0')// file_src - адрес (типа char *), где должны сохраняться введенные данные
    {
        if (strcmp(file_src, file_app) == 0)
            fputs("Can't append file to itself\n",stderr);
        else if ((fs = fopen(file_src, "r")) == NULL)
            fprintf(stderr, "Can't open %s\n", file_src);
        else
        {
            if (setvbuf(fs, NULL, _IOFBF, BUFSIZE) != 0)
            {
                fputs("Can't create input buffer\n",stderr);
                continue;
            }
            append(fs, fa);
            if (ferror(fs) != 0)
                fprintf(stderr,"Error in reading file %s.\n",
                        file_src);
            if (ferror(fa) != 0)
                fprintf(stderr,"Error in writing file %s.\n",
                        file_app);
            fclose(fs);
            files++;
            printf("File %s appended.\n", file_src);
            puts("Next file (empty line to quit):"); // имя следующего файла источника данных обрабатывается в цикле while нашей функцией s_gets которая использует стандартную функцию fgets	
        }
    }
    printf("Done appending. %d files appended.\n", files);
    rewind(fa);
    printf("%s contents:\n", file_app);
    while ((ch = getc(fa)) != EOF)
        putchar(ch);
    puts("Done displaying.");
    fclose(fa);
    
    return 0;
}

void append(FILE *source, FILE *dest)
{
    size_t bytes;
    static char temp[BUFSIZE]; // allocate once // выделить память один раз
    //static char temp[]= "This is empty string"; // allocate once // выделить память один раз
    
	// Аргумент size представляет размер в байтах порции данных, подлежащих чтению/записи, а nmemb(третий аргумент) — количество таких порций.
    //bytes=fread(temp,sizeof(char),BUFSIZE,source); // #define BUFSIZE 4096
    bytes=fread(temp,sizeof(char),5,source); // #define BUFSIZE 4096 // читаем только пять порций по байту(т.к.char)
	fprintf(stdout, "Read %d\n", bytes);
    bytes =  fwrite(temp, sizeof (char), 32, dest); // пишем из temp в файл 32 порции по байту, если temp содержит меньше элементов чем 32, то добавляются пустые строки до bytes = 32
	fprintf(stdout, "Write %d\n", bytes);
}

char * s_gets(char * st, int n)
{
    char * ret_val;
    char * find;
    
	//fgets предназначена для файлового ввода, первым аргументом является адрес (типа char *), где должны сохраняться введенные данные. Второй аргумент - целое число, представляющее максимальный размер входной строки. Заключительный аргумент - это указатель файла, который идентифицирует файл, подлежащий чтению. итает входные данные до появления первого символа новой строки - до тех пор, пока не будет прочитано количество символов, на единицу меньше верхнего предела, либо пока не будет обнаружен конец файла; затем fgets () добавляет завершающий нулевой символ, чтобы сформировать строку. 
	// Функция fgets () возвращает указатель на char. Если все проходит нормально, она просто возвращает тот же адрес, который был ей передан в первом аргументе. Однако если функция встречает конец файла, она возвращает специальный указатель, называемый нулевым
    ret_val = fgets(st, n, stdin);
    if (ret_val)
    {
        find = strchr(st, '\n');   // look for newline
        if (find)                  // if the address is not NULL,
            *find = '\0';          // place a null character there
        else
            while (getchar() != '\n')
                continue;
    }
    return ret_val;
}

