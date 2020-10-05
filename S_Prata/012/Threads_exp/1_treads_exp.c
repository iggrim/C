#include <stdatomic.h> // F:\MSYS2_64\mingw64\lib\gcc\x86_64-w64-mingw32\9.2.0\include\
#include <stdio.h>

int main(void)
{
	/*  int hogs; // обычное объявление
        hogs = 12; // обычное присваивание можно было бы заменить следующим кодом:
	*/
	_Atomic int hogs; // hogs - атомарная переменная
	atomic_store(&hogs, 12); // макрос из stdatomic.h
}

// срабатывает