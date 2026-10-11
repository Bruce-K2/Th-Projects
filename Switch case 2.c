#include <stdio.h>
int main()
{
    float no1, no2 ,ans;
    char operation;
    printf("Enter the two numbers\n");
    scanf("%f%f",&no1,no2);
    printf("Enter the operation to be performed(either +,-,/,* )");
    scanf("%c", &operation);
    switch(operation)
    {
        case '+':
        ans = no1+no2;
        break;
        case '-':
        ans = no1-no2;
        break;
        case '/':
        ans = no1/no2;
        break;
        case '*':
        ans = no1*no2;
        break;
        default:
        printf("Wrong operatio entered\n");
        ans = 0;
        break;
    }
    printf("%10.2f %c %10.2f =%10.2f", no1, operation, no2, ans);
    return 0;
}