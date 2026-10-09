// Write code which displays numbers stored in a an array
#include <stdio.h>
int main()
{


    int a [5][2] = {{200,130},
                    {120,700},
                    {400,350},
                    {234,687},
                    {412,845} }; // an array with 5 rows and 2 columns
    int i, j;
    for (i = 0; i < 5;  i++)
    {
      
        for (j = 0; j < 2; j++)
        {
            printf("a[%d] [%d] = %d\n", i , j ,a[i][j]);
        }
    }
    return 0;
}

