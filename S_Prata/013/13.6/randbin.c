/* randbin.c -- random access, binary i/o */
#include <stdio.h>
#include <stdlib.h>
#define ARSIZE 1000

int main()
{
    double numbers[ARSIZE];
    double value;
    const char * file = "numbers.dat";
    int i;
    long pos;
    FILE *iofile;
	size_t bytes;
    
    // create a set of double values
    for(i = 0; i < ARSIZE; i++)
        numbers[i] = 100.0 * i + 1.0 / (i + 1); // 100,5 ,  200,3333333333333 и т.д.
    // attempt to open file
    if ((iofile = fopen(file, "wb")) == NULL) // двоичный режим на запись
    {
        fprintf(stderr, "Could not open %s for output.\n", file);
        exit(EXIT_FAILURE);
    }
    // write array in binary format to file
	// size_t fwrite(const void * restrict ptr, size_t size, size_t nmemb, FILE * restrict fp); Функция fwrite() записывает двоичные данные в файл. Возвращает количество записаных блоков. Тип size_ t определен в терминах стандартных типов С. Это тип, возвращаемый операцией sizeof. Обычно им является unsigned int, но реализации могут выбирать другой тип.Указатель ptr это адрес порции данных, предназначенной для записи. Аргумент size представляет размер в байтах порции данных, подлежащих записи, а nmemb количество таких порций (ЗА РАЗ). Как обычно, fp идентифицирует файл, в который должна производиться запись.
    bytes = fwrite(numbers, sizeof (double), ARSIZE, iofile);
	fprintf(stdout, "Write %d\n", bytes);
    fclose(iofile);
    if ((iofile = fopen(file, "rb")) == NULL)// двоичный режим на чтение
    {
        fprintf(stderr,
                "Could not open %s for random access.\n", file);
        exit(EXIT_FAILURE);
    }
    // read selected items from file
    printf("Enter an index in the range 0-%d.\n", ARSIZE - 1);
    while (scanf("%d", &i) == 1 && i >= 0 && i < ARSIZE)
    {
        pos = (long) i * sizeof(double); // calculate offset //вычисление смещения
        fseek(iofile, pos, SEEK_SET); // go there // переход в нужную позицию // Откуда измеряется смещение - SEEK_SET - от начала файла 
        fread(&value, sizeof (double), 1, iofile);
        printf("The value there is %f.\n", value);
        printf("Next index (out of range to quit):\n");
    }
    // finish up
    fclose(iofile);
    puts("Bye!");
    
    return 0;
}
