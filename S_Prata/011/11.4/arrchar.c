//  arrchar.c -- array of pointers, array of strings
#include <stdio.h>
#define SLEN 40
#define LIM 5
int main(void)
{
    const char *mytalents[LIM] = {
        "Adding numbers swiftly",
        "Multiplying accurately", "Stashing data",
        "Following instructions to the letter",
        "Understanding the C language"
    };
    char yourtalents[LIM][SLEN] = {
        "Walking in a straight line",
        "Sleeping", "Watching television",
        "Mailing letters", "Reading email"
    };
    int i;
    
    puts("Let's compare talents.");
    printf ("%-36s  %-25s\n", "My Talents", "Your Talents");
    for (i = 0; i < LIM; i++)
        printf("%-36s  %-25s\n", mytalents[i], yourtalents[i]);
    printf("\nsizeof mytalents: %d, sizeof yourtalents: %d\n\n",
           sizeof(mytalents), sizeof(yourtalents));

	printf("mytalents[1][2]=%c  yourtalents[1][2]=%c\n", mytalents[1][2], yourtalents[1][2]);
	printf("mytalents[1][3]=%c  yourtalents[1][3]=%c\n", mytalents[1][3], yourtalents[1][3]);
    
	/* mytalents[1][2] = 'D';
	printf("mytalents[1][2]=%c\n", mytalents[1][2]); 
	======на этапе компиляции генерируется ошибка=====
    error: assignment of read-only location '*(mytalents[1] + 2)
     */
	yourtalents[1][1]= 'S';
	//printf("yourtalents[1][2]=%c\n", yourtalents[1][2]); 
	
	for (i = 0; i < LIM; i++)
        printf("%-36s  %-25s\n", mytalents[i], yourtalents[i]);
	return 0;
}
/*
Let's compare talents.
My Talents                            Your Talents
Adding numbers swiftly                Walking in a straight line
Multiplying accurately                Sleeping
Stashing data                         Watching television
Following instructions to the letter  Mailing letters
Understanding the C language          Reading email

sizeof mytalents: 40, sizeof yourtalents: 200

mytalents[1][2]=l  yourtalents[1][2]=e
mytalents[1][3]=t  yourtalents[1][3]=e

Adding numbers swiftly                Walking in a straight line
Multiplying accurately       ----->   SSeeping   <-------
Stashing data                         Watching television
Following instructions to the letter  Mailing letters
Understanding the C language          Reading email
*/



