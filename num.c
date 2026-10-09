#include <stdio.h>
int main()
{
   int marks[5];

   //Read
   printf("Enter 5 marks: \n");
   for(int i = 0;i<5;i++)
   
    {printf("Mark %d:",i + 1);
     scanf("%d", &marks[i]);}

   //Display
   printf("You entered:");
   for(int i = 0;i<5;i++)
   {printf("%d\n", marks[i]);}
   
}