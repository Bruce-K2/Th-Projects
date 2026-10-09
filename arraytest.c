#include <stdio.h>

int main() {
    // Declare and initialize a 3x4 2D array
    int matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    int rows = 3;
    int cols = 4;

    // Display the array
    printf("2D Array (%d x %d):\n\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n"); // New line after each row
    }

    // Display a 3D array (2x3x2)
    int cube[2][3][2] = {
        {{1, 2}, {3, 4}, {5, 6}},
        {{7, 8}, {9, 10}, {11, 12}}
    };

    printf("\n3D Array (2 x 3 x 2):\n\n");
    for (int i = 0; i < 2; i++) {
        printf("Matrix %d:\n", i + 1);
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 2; k++) {
                printf("%4d", cube[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }

    return 0;
}