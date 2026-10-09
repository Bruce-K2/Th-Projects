#include <stdio.h>
int main()
{
    int sum =0, average = 0;
    int nums[10];
    // REad
    for(int i = 0; i < 10; i++)
    {
     printf("Nombre %d:", i + 1);
     scanf("%d", &nums[i]);
    }
    //Computeer
    for (int i = 0;i < 10;i++)
    {
        sum += nums[i];
        average = sum/10;
    }
printf("SUm is %d\n", sum);
printf("Average isu: %d\n", average);
}

