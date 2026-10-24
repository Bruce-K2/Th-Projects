//Write a program in C that reads the student name and membership number using fgets() and prints it back to the console.Here is a simple C program that reads a full string using `fgets()` and prints it back for library:
#include <stdio.h>
#include <string.h>

int main() {
    char fullName[100];
    char membership[50];

    printf("Enter student name: ");
    fgets(fullName, sizeof(fullName), stdin);
    fullName[strcspn(fullName, "\n")] = 0; // Remove newline character if present
    
    printf("Enter membership number: ");
    fgets(membership, sizeof(membership), stdin);
    membership[strlen(membership) - 1 ] = '\0'; // Remove newline character if present
    
    printf("Student Name is: %s\n", fullName);
    printf("Membership Number: %s\n", membership);
    
    return 0;
}
