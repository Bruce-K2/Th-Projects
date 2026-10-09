//Read marks and find highest, lowest, total and average.
#include <stdio.h>
int main()
{
    
    int marks[10], total = 0, average = 0;
    int highest, lowest;
    int i =0;
    //Read 10 Marks
for(i = 0;i < 10; i++)
    {
        printf("Mark %d:",i + 1);
        scanf("%d", &marks[i]);
    }

    //Computing the total
    for(i = 0;i < 10;i++)
    {
        total += marks[i];
        average = total/10;
    }

    //Initialize highest and lowest from first element
    highest = marks[0];
    lowest = marks[0];
    //Find highest and lowest
    for(i = 0; i < 10;i++)
    {if (marks[i] > highest) highest = marks[i];
     if (marks[i] < lowest) lowest = marks[i]; }

      
     printf("Total: %d\n", total);
     printf("Highest: %d\n", highest);
     printf("Lowest: %d\n", lowest);
     printf("Average: %d\n", average);
    
    return 0;
    

} 