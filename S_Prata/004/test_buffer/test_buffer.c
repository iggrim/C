#include <stdio.h>
int main(void)
{
	int num_0;
    char num_1; 
	int num_2;
    char ch1, ch2;
    
    printf("Enter a number :\n");	
	
	scanf("%d", &num_0);
	printf("The  num0  is :%d\n", num_0); 
	
	scanf("%c", &num_1);
	printf("The  num1  is :%c\n", num_1); 
	
	ch1 = getchar(); 
    printf("The number char1 is :%c\n", ch1);
	
    ch2 = getchar();
    printf("The number char2 is :%c\n", ch2); /**/
    
    scanf("%d", &num_2);
	printf("The  num2  is :%d\n", num_2); 
	
    return 0;
}
/*
и scanf и getchar читают из буфера ввода и очищают его
*/