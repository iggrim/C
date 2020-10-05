/* zippo1.c --  zippo info */
#include <stdio.h>
int main(void)
{
    int zippo[4][2] = { {2,4}, {6,8}, {1,3}, {5, 7} };
    
    printf("   zippo = %p,    zippo + 1 = %p\n", zippo, zippo + 1);
    printf("   *zippo + 2 = %p\n", *zippo + 2); // *zippo+2 = zippo+1
	
    printf("zippo[0] = %p, zippo[0] + 1 = %p\n", zippo[0],  zippo[0] + 1);
    printf("  *zippo = %p,   *zippo + 1 = %p\n", *zippo, *zippo + 1);
    printf("zippo[0][0] = %d\n", zippo[0][0]);
    printf("  *zippo[0] = %d\n", *zippo[0]);
    printf("    **zippo = %d\n", **zippo);
    printf("      zippo[2][1] = %d\n", zippo[2][1]);
    printf("*(*(zippo+2) + 1) = %d\n", *(*(zippo+2) + 1));
    
    return 0;
}
/*
   zippo = 000000000022FE30,    zippo + 1 = 000000000022FE38
   *zippo + 2 = 000000000022FE38
zippo[0] = 000000000022FE30, zippo[0] + 1 = 000000000022FE34
  *zippo = 000000000022FE30,   *zippo + 1 = 000000000022FE34
zippo[0][0] = 2
  *zippo[0] = 2
    **zippo = 2
      zippo[2][1] = 3
*(*(zippo+2) + 1) = 3
*/
