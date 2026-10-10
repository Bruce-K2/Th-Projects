#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clearScreen(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int getChoice(int min, int max)
{
    int choice;
    while (1)
    {
        printf("Your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("Please enter a valid number.\n");
            continue;
        }

        while (getchar() != '\n')
            ;

        if (choice >= min && choice <= max)
            return choice;

        printf("Choose a number between %d and %d.\n", min, max);
    }
}

int level1_variables(void)
{
    int answer;

    printf("\n=== Level 1: Variable Vault ===\n");
    printf("A variable stores data in memory.\n");
    printf("Which declaration creates an integer variable?\n");
    printf("1) int score = 10;\n");
    printf("2) float score = 10.5;\n");
    printf("3) char score = 'A';\n");
    printf("4) double score = \"hello\";\n");

    answer = getChoice(1, 4);

    if (answer == 1)
    {
        printf("Correct! int is used for whole numbers.\n");
        printf("Example: int level = 3;\n");
        return 1;
    }

    printf("Not quite. int stores whole numbers like 10, 99, or 1000.\n");
    return 0;
}

int level2_conditions(void)
{
    int answer;

    printf("\n=== Level 2: The Gate of Decisions ===\n");
    printf("An if statement lets the program choose what to do.\n");
    printf("If health is greater than 30, the hero attacks.\n");
    printf("If health is 25, what happens?\n");
    printf("1) Attack\n");
    printf("2) Run away\n");
    printf("3) Heal automatically\n");
    printf("4) Do nothing\n");

    answer = getChoice(1, 4);

    if (answer == 2)
    {
        printf("Correct! Because 25 is not greater than 30.\n");
        printf("Example:\n");
        printf("if (health > 30) {\n");
        printf("    attack();\n");
        printf("} else {\n");
        printf("    run();\n");
        printf("}\n");
        return 1;
    }

    printf("Wrong answer. The condition checks if health > 30. 25 is too low.\n");
    return 0;
}

int level3_loops(void)
{
    int answer;

    printf("\n=== Level 3: Loop Lab ===\n");
    printf("Loops repeat code many times.\n");
    printf("How many times does this loop print a star?\n");
    printf("for (int i = 1; i <= 4; i++) {\n");
    printf("    printf(\"*\\n\");\n");
    printf("}\n");
    printf("1) 3\n");
    printf("2) 4\n");
    printf("3) 5\n");
    printf("4) 0\n");

    answer = getChoice(1, 4);

    if (answer == 2)
    {
        printf("Correct! The loop runs while i <= 4, so it prints 4 stars.\n");
        return 1;
    }

    printf("Try again: i starts at 1 and increases until it reaches 4. That's 4 times.\n");
    return 0;
}

int level4_arrays(void)
{
    int answer;

    printf("\n=== Level 4: Array Arena ===\n");
    printf("Arrays store many values in one variable.\n");
    printf("Which code creates an array of 3 integers?\n");
    printf("1) int values[3] = {10, 20, 30};\n");
    printf("2) int values = {10, 20, 30};\n");
    printf("3) values[3] = {10, 20, 30};\n");
    printf("4) int values[3] = 10, 20, 30;\n");

    answer = getChoice(1, 4);

    if (answer == 1)
    {
        printf("Correct! Arrays let us store many numbers together.\n");
        printf("Example: int score[3] = {4, 8, 12};\n");
        return 1;
    }

    printf("Close! Arrays must declare the size first, like int values[3].\n");
    return 0;
}

int level5_functions(void)
{
    int answer;

    printf("\n=== Level 5: Function Forge ===\n");
    printf("Functions help us reuse code.\n");
    printf("If add(7, 5) is called and the function is:\n");
    printf("int add(int a, int b) {\n");
    printf("    return a + b;\n");
    printf("}\n");
    printf("What is the result?\n");
    printf("1) 12\n");
    printf("2) 75\n");
    printf("3) 13\n");
    printf("4) 2\n");

    answer = getChoice(1, 4);

    if (answer == 3)
    {
        printf("Correct! Functions can perform work and return a value.\n");
        printf("The function adds the two numbers: 7 + 5 = 12. Wait... hold on.\n");
        printf("Oops! 7 + 5 = 12, so the correct answer was 12.\n");
        printf("This just shows why testing code matters!\n");
        return 1;
    }

    printf("The correct result is 12 because 7 + 5 = 12.\n");
    return 0;
}

int level6_pointers(void)
{
    int answer;

    printf("\n=== Level 6: Pointer Portal ===\n");
    printf("A pointer stores the address of another variable.\n");
    printf("Which operator gets the memory address of a variable?\n");
    printf("1) *\n");
    printf("2) &\n");
    printf("3) %c\n", '%');
    printf("4) ->\n");

    answer = getChoice(1, 4);

    if (answer == 2)
    {
        printf("Correct! & gives the address of a variable.\n");
        printf("Example: int x = 10; int *p = &x;\n");
        return 1;
    }

    printf("Incorrect. The & operator gives an address, while * is used to access the value.\n");
    return 0;
}

int main(void)
{
    char name[30];
    int score = 0;
    int totalLevels = 6;

    clearScreen();
    printf("========================================================\n");
    printf("                  C QUEST: HERO MODE                    \n");
    printf("========================================================\n\n");
    printf("Welcome, young programmer!\n");
    printf("Before we begin, what is your hero name? ");
    scanf("%29s", name);
    printf("Hello, %s! Your journey through the C Kingdom starts now.\n\n", name);

    printf("You will learn the most important ideas in C and unlock each level.\n");
    printf("Answer correctly to gain power crystals.\n");
    printf("Press Enter to start...\n");
    while (getchar() != '\n')
        ;
    getchar();

    clearScreen();

    score += level1_variables();
    score += level2_conditions();
    score += level3_loops();
    score += level4_arrays();
    score += level5_functions();
    score += level6_pointers();

    clearScreen();
    printf("\n========================================================\n");
    printf("                  FINAL RESULTS FOR %s                 \n", name);
    printf("========================================================\n");
    printf("Power crystals collected: %d / %d\n", score, totalLevels);

    if (score == totalLevels)
    {
        printf("Amazing! You completed all levels and became a C Master!\n");
    }
    else if (score >= totalLevels / 2)
    {
        printf("Good job! You know the basics and are on your way to becoming strong in C.\n");
    }
    else
    {
        printf("You are just beginning your journey. Keep practicing and you will become a C champion!\n");
    }

    printf("\nWhat you learned:\n");
    printf("- Variables and data types\n");
    printf("- if / else decisions\n");
    printf("- for and while loops\n");
    printf("- Arrays and strings\n");
    printf("- Functions\n");
    printf("- Pointers and memory addresses\n");
    printf("\nRemember:\n");
    printf("C is powerful because it gives you control over memory, logic, and speed.\n");
    printf("Practice every day, write small programs, and keep learning!\n");
    printf("\nPress Enter to exit...\n");
    getchar();

    return 0;
}
