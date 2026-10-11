#include <stdio.h>
int main()
{
    int choice;
    float area;
    printf("\n\nChoose the shape to compute th area\n1. Circle, \n2. Rectangle");
    printf("\n3. Trapezium \n4. Square \n"  );
    scanf("%d", &choice);
    switch(choice)
    {
        case 1:
        float r;
        printf("\nArea of a circle \nEnter the radiu of the circle: ");
        scanf("%f",&r);
        area = (22/7)*r*r;
        break;
        case 2:
        int l,w;
        printf("\nArea of a rectangle \nEnter the length and width:");
        scanf("%d%d",&l,&w);
        break;
        case 3:
        int a, b, h;
        printf("\nArea of a Trapezium \nEnter the length, width and height:");
        scanf("%d%d%d",&a, &b, &h);
        area = 0.5*(a+b)*h;
        break;
        case 4:
        int s;
        printf("\n Area of a square \nEnter the length of the square");
        scanf("%d",&s);
        area = s*s;
        break;
        default:
        area = 0;
        printf("\nWrong shape chosen");
        break;
    }
    printf("\n\nThe area of the shape is %2f", area);
    return 0;
    
}