//A secondary school wants a program to record marks for 10 students in a class and produce a summary report. The program must read the marks into an array, compute the total and average,find the highest and lowest marks, and display all results.*/
#include <stdio.h>
 
int main()
{
    int marks [10];
    int total = 0;
    float average = 0;
    for(int i =0; i<10;i++)
{
    printf("Enter Mark %d :",i + 1);
    scanf("%d", &marks[i]);
    total +=  marks[i];
    
}
   average = total / 10.0; 
   printf("The total marks is :%d\n", total);
   printf("The average marks is :%.2f\n", average);
   
   return 0;
