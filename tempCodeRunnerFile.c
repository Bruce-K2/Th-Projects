#include <stdio.h>
int add(int a, int b) ;
int multiply(int x, int y) ;

int main() {
     
    int sumOutput = add(45, 10);
    printf("The sum is: %d\n", sumOutput);
     
    int mult = multiply(5, 1342);
    printf("The product is: %d\n", mult);

}  

int add(int a, int b) {
    return a + b;
}

int multiply(int x, int y) {
    return x * y;
}
    