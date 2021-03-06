/* dualview.c -- bit fields and bitwise operators */
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
/* BIT-FIELD CONSTANTS */
/* line styles     */
#define SOLID   0
#define DOTTED  1
#define DASHED  2
/* primary colors  */
#define BLUE    4
#define GREEN   2
#define RED     1
/* mixed colors    */
#define BLACK   0
#define YELLOW  (RED | GREEN)
#define MAGENTA (RED | BLUE)
#define CYAN    (GREEN | BLUE)
#define WHITE   (RED | GREEN | BLUE)

/* BITWISE CONSTANTS   */
#define OPAQUE          0x1
#define FILL_BLUE       0x8
#define FILL_GREEN      0x4
#define FILL_RED        0x2
#define FILL_MASK       0xE
#define BORDER          0x100
#define BORDER_BLUE     0x800
#define BORDER_GREEN    0x400
#define BORDER_RED      0x200
#define BORDER_MASK     0xE00
#define B_SOLID         0
#define B_DOTTED        0x1000
#define B_DASHED        0x2000
#define STYLE_MASK      0x3000

const char * colors[8] = {"black", "red", "green", "yellow",
    "blue", "magenta", "cyan", "white"};
struct box_props {

    bool opaque                 : 1;
    unsigned int fill_color     : 3;
    unsigned int                : 4;
    bool show_border            : 1;
    unsigned int border_color   : 3;
    unsigned int border_style   : 2;
    unsigned int                : 2;
};

union Views     /* look at data as struct or as unsigned short */
{
    struct box_props st_view;
    unsigned short us_view; // по коду ниже инициализируется как true, т.е. 1
    //int test;
    //struct box_props st_test; // по коду ниже инициализируется как и st_view
};

// При желании имена переменных ( формальные параметры ) в прототипе можно не указывать
void show_settings(const struct box_props * pb); // функция, основана на представлении структуры
void show_settings1(unsigned short); // функция, основана на представлени unsigned short


char * itobs(int n, char * ps);

int main(void)
{
    /*Объединения - это объект, позволяющий нескольким переменным различных типов занимать один  участок памяти. Когда объявлено объединение, компилятор автоматически создает переменную достаточного размера для хранения наибольшей переменной, присутствующей в объединении.
    При определении переменной объединения мы ее можем сразу инициализировать, но стоит учитывать, что инициализировать мы можем только первый элемент объединения.
    int value = {{1, 1}};или более коротко: int value = {1, 1};*/
    /* create Views object, initialize struct box view */

	// =======создание объекта Views, инициализация представления в виде структуры======
    union Views box = {{true, YELLOW , true, GREEN, DASHED}}; // #define YELLOW  (RED | GREEN)//1 2
    // эксперементально установлено, что при такой записи инициализации объединения
    // инициализируется первый член объединения (структура),
    // а последующие члены (простые типы) инициализируются первым значением - как true, т.е. еденицей
    char bin_str[8 * sizeof(unsigned int) + 1];

	//====================

    // Исходные настройки окна:
    printf("Original box settings:\n");
	// функция, основана на представлении структуры
    show_settings(&box.st_view);  // ===================union Views box

	// Настройки окна с использованием представления unsigned short(до изменения):
    printf("\nBox settings using unsigned int view (show_settings1) :\n");
	// побитовых операции: логические операции и операции сдвига.
    show_settings1(box.us_view); // ====================unsigned short us_view;

    printf("bits are %s\n",
        	itobs(box.us_view,bin_str));

	//=================== изменяем настройки
    box.us_view &= ~FILL_MASK;   // clear fill bits // #define FILL_MASK    0xE
    box.us_view |= (FILL_BLUE | FILL_GREEN); /* reset fill */
    box.us_view ^= OPAQUE;               /* toggle opacity */
    box.us_view |= BORDER_RED;           /* wrong approach */
    box.us_view &= ~STYLE_MASK;        /* clear style bits */
    box.us_view |= B_DOTTED;        /* set style to dotted */

	// Измененные настройки окна:
    printf("\nModified box settings:\n");
	// функция, основана на представлении структуры
    show_settings(&box.st_view);

	// Настройки окна с использованием представления unsigned short(после изменения):
    printf("\nBox settings using unsigned int view:\n");

	//побитовых операции: логические операции и операции сдвига.
    show_settings1(box.us_view); // Фактический аргумент — это выражение, указанное в круглых скобках при вызове функции. Формальный параметр - это переменная, объявленная в заголовке определения функции. Когда функция вызывается, переменные, объявленные как формальные параметры, создаются и инициализируются значениями, которые получены в результате вычисления фактических аргументов.

    printf("bits are %s\n",
           itobs(box.us_view,bin_str));

    return 0;
}
// значения для pb->opaque и т.д. беруться из union Views box = {{true, YELLOW , true, GREEN, DASHED}};
void show_settings(const struct box_props * pb)
{
    printf("Box is %s.\n",
           pb->opaque == true ? "opaque": "transparent");
    printf("The fill color is %s.\n",
           colors[pb->fill_color]);
    printf("Border %s.\n",
           pb->show_border == true ? "shown" : "not shown");
    printf("The border color is %s.\n",
           colors[pb->border_color]);
    printf ("The border style is ");
    switch(pb->border_style)
    {
        case SOLID  : printf("solid.\n"); break;
        case DOTTED : printf("dotted.\n"); break;
        case DASHED : printf("dashed.\n"); break; // #define DASHED  2
        default     : printf("unknown type.\n");
    }
}

// побитовых операции: логические операции и операции сдвига.
// const char * colors[8] = {"black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"};
void show_settings1(unsigned short us)
{
    printf("box is %s.\n",
           (us & OPAQUE) == OPAQUE? "opaque": "transparent"); // #define OPAQUE  0x1
    printf("The fill color is %s.\n",
           colors[(us >> 1) & 07]); //0111
    printf("Border %s.\n",
           (us & BORDER) == BORDER? "shown" : "not shown"); // #define BORDER  0x100
    printf ("The border style is ");
    switch(us & STYLE_MASK) // #define STYLE_MASK    0x3000
    {
        case B_SOLID  : printf("solid.\n"); break; // #define B_SOLID  0
        case B_DOTTED : printf("dotted.\n"); break;
        case B_DASHED : printf("dashed.\n"); break; // #define B_DASHED  0x2000
        default       : printf("unknown type.\n");
    }
    printf("The border color is %s.\n",
           colors[(us >> 9) & 07]); //0111

}

char * itobs(int n, char * ps) // n=1
{
    int i;
    const static int size = CHAR_BIT * sizeof(int); // 8(бит) умножить на 4(байта) = 32

    for (i = size - 1; i >= 0; i--, n >>= 1)
        ps[i] = (01 & n) + '0'; // первоначально n=1 и следовательно (01 & n) тоже 1, добаление символа '0' преобразует числов в символ
    ps[size] = '\0';

    return ps;
}
