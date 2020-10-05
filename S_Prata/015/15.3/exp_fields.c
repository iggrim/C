/* fields.c -- define and use fields */
#include <stdio.h>
#include <stdbool.h>   //C99, defines bool, true, false

#define SOLID   0
//#define SOLID  3
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
//#define MAGENTA 1000
#define CYAN    (GREEN | BLUE)
#define WHITE   (RED | GREEN | BLUE)


struct box_props {
	unsigned int test1         : 1;
	unsigned int test2           : 1;
	unsigned int test3           : 30;
	//unsigned int test4           : 1; 
    //bool opaque                 : 1;  // or unsigned int (pre C99)
     //unsigned int fill_color     : 32;
    //unsigned int                : 4;
    //bool show_border            : 1;  // or unsigned int (pre C99) 
    //unsigned int border_color   : 1;
    //unsigned int _style   : 1; 
    //unsigned int                : 2;  
};

int main(void)
{
    /* create and initialize box_props structure */
    //struct box_props box = {true, YELLOW , true, GREEN, DASHED};
    //struct box_props box = {1};
    struct box_props box;
    /* 
	box.opaque = false;
    box.fill_color = WHITE;
    box.border_color = MAGENTA;
    box.border_style = SOLID; // если больше трех, то ошибка */
	
	// перация sizeof, которая возвращает размер типав байтах.
    printf("Size of structure box :\n");
    printf("%d\n", sizeof(box)); // если в структуре < 32 бит, то 4 байта, если >= 32, то 8 байт
	/*
	printf("field the box.fill_color :\n");
	printf("%d\n", box.fill_color); // 7
	
	printf("field the box.border_style :\n");
	printf("%d\n", box.border_style); // 0
	
	printf("field the box.border_color :\n");
	printf("%d\n", box.border_color); //  */
	    
    return 0;
}


