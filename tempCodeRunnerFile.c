#include <stdio.h>

int main(void)
{
    int choice;
    float area = 0;

    printf("Choose the shape to compute the area:\n");
    printf("1. Circle\n2. Rectangle\n3. Trapezium\n4. Square\n");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1: {
            float r;
            printf("Area of a circle. Enter the radius: ");
            scanf("%f", &r);
            area = (22.0f / 7.0f) * r * r;
            break;
        }
        case 2: {
            float l, w;
            printf("Area of a rectangle. Enter length and width: ");
            scanf("%f%f", &l, &w);
            area = l * w;
            break;
        }
        case 3: {
            float a, b, h;
            printf("Area of a trapezium. Enter side a, side b, and height: ");
            scanf("%f%f%f", &a, &b, &h);
            area = 0.5f * (a + b) * h;
            break;
        }
        case 4: {
            float s;
            printf("Area of a square. Enter the side length: ");
            scanf("%f", &s);
            area = s * s;
            break;
        }
        default:
            area = 0;
            printf("Wrong shape chosen\n");
            break;
    }

    printf("The area of the shape is %.2f\n", area);
    return 0;
}