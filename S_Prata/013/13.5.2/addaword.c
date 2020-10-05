#include <stdio.h>
#include <stdlib.h>
#include <Windows.h> // SetConsoleCP, SetConsoleOutputCP
#include <time.h>

void filecpy(FILE *stream_from, FILE *stream_to);

// Буфера для файлов

char buf1[BUFSIZ * 10];
char buf2[BUFSIZ * 10];


void main(int argc, char *argv[]) {
	// =======файл должен быть сохранен в кодировке ANSI=======
	SetConsoleCP(1251); // <Windows.h> Функция SetConsoleCP устанавливает кодовую страницу ввода
    SetConsoleOutputCP(1251);// <Windows.h>

         time_t start, end;
         FILE *stream_from, *stream_to;


         if(argc < 3) {
                  printf("Задайте имена файлов!\n");
                  exit(-1);
         }

// Открываем файлы и используем для копирования
// буфер стандартного размера

         if((stream_from = fopen(argv[1], "rt")) == NULL) {
                  printf("Задайте имя входного файла!\n"); // первоначально
                  exit(-1);
         }
         stream_to = fopen(argv[2], "wt+");

// Определяем время начала копирования

         start = clock();

// Выполняем копирование файла

         filecpy(stream_from,stream_to);

// Определяем время завершения копирования

         end = clock();

// Выводим время копирования при использовании
// буферов стандартного размера

         printf("Время копирования: %5.1f Размер буфера, байтов: %d\n",
                 ((float)end - start) / CLK_TCK, BUFSIZ);


// Задаем свой буфер большего размера

         if((stream_from = fopen(argv[1], "rt")) == NULL) exit(-1);
         stream_to = fopen(argv[2], "wt+");

// Устанавливаем буфера как для входного,
// так и для выходного файлов

         setvbuf(stream_from, buf1, _IOFBF, sizeof(buf1));
         setvbuf(stream_to,   buf2, _IOFBF, sizeof(buf2));

// Копируем файл и измеряем продолжительность
// копирования

         start = clock();
         filecpy(stream_from,stream_to);
         end = clock();

         printf("Время копирования: %5.10f Размер буфера: %d\n",
                 ((float)end - start) / CLK_TCK, BUFSIZ * 10);

// Копируем без использования буферизации

         if((stream_from = fopen(argv[1], "rt")) == NULL) exit(-1);
         stream_to = fopen(argv[2], "wt+");

         setvbuf(stream_from, NULL, _IONBF, 0);
         setvbuf(stream_to,   NULL, _IONBF, 0);

         start = clock();
         filecpy(stream_from,stream_to);
         end = clock();

         printf("Время копирования: %5.1f Буферизация не используется\n",
                 ((float)end - start) / CLK_TCK);

         exit(0);
}

// Функция для копирования файлов

void filecpy(FILE *stream_from, FILE *stream_to) {

         char linebuf[256];

// Цикл копирования. Условие выхода из цикла -
// конец входного файла
// Функция feof () возвращает ненулевое значение, если при последнем вызове функции ввода б ы л обнаружен конец файла, и ноль в противном случае. 
         while(!feof(stream_from)) {

// Читаем в буфер linebuf одну строку

                  if(fgets(linebuf, 255, stream_from) == NULL) break;

// Записываем содержимое буфера linebuf
// в выходной файл

                  if(fputs(linebuf, stream_to) != 0) break;
         }

// Закрываем входной и выходной файлы

         fclose(stream_from);
         fclose(stream_to);
}

/* F:\Temp\C_lang\!_S_Prata\013\13.5.2>add.exe wordy dest
Время копирования:   0.0 Размер буфера, байтов: 512
Время копирования: 0.0010000000 Размер буфера: 5120
Время копирования:   0.0 Буферизация не используется */