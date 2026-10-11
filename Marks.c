#include <stdio.h>
int main()
{
    int marks;
    printf("Enter your marks");
    scanf("%i",&marks);
    if(marks>=89&&marks<=100)
     {
        printf("A");
     }
    else if(marks>=80&&marks<=88)
     {
        printf("B");     
     }
    else if(marks>=70&&marks<=79)
     {
        printf("C");
     } 
    else if(marks>=60&&marks<=69)
    {
        printf("D");
    }
    else{
        printf("FAILURE, DoGWater");
    }
    return 0;
}