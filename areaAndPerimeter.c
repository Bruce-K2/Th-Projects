//Write a C program that declares two functions, area(len, wid) and per(len, wid), to calculate the area and perimeter of a rectangle respectively. The program should prompt the user to enter the length and width, call the functions, and display the results
#include <stdio.h>
 
// Function to calculate area of rectangle
float area(float len, float wid) {
    return len * wid;
}   

// Function to calculate perimeter of rectangle
float per(float len, float wid) {
    return 2 * (len + wid);
}

int main() {
    float length, width;

    printf("Enter length of rectangle: ");
    scanf("%f", &length);

    printf("Enter width of rectangle: ");
    scanf("%f", &width);

    printf("Area of rectangle: %.2f\n", area(length, width));
    printf("Perimeter of rectangle: %.2f\n", per(length, width));

    return 0;
}