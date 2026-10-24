#include <stdio.h>
int add(int a, int b) ;       // Function declaration for add
int mult(int x, int y) ;  // Function declaration for multiply
void printHello() ;           // Void function declaration for printHello
    
int main() {
     
    int sumOutput = add(45, 10);          //Calling the add function and printing the result
    printf("The sum is: %d\n", sumOutput);
     
    int multiply = mult(5, 1342);         //Calling the multiply function and printing the result
    printf("The product is: %d\n", multiply); 
    printHello();
}  

int add(int a, int b) {    //Function definition for add
    return a + b;
}

int mult(int x, int y) { //Function definition for multiply
    return x * y;
}
    
void printHello() {          //Function definition for printHello
    printf("Hello, World!\n");
}