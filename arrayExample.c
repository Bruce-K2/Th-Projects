#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ATTEMPTS 7
#define NUM_GAMES 3

int main() 
{


    int targets[NUM_GAMES];      // secret numbers for each round
    int guesses[MAX_ATTEMPTS];   // store all guesses for current round
    int attempts = 0;
    int round, i;
    int won = 0;

    srand(time(0));

    // Generate secret numbers for each round
    for (round = 0; round < NUM_GAMES; round++) 
    {
        targets[round] = rand() % 100 + 1;
    }

    printf("=== Guess the Number Game ===\n");
    printf("You will play %d rounds.\n", NUM_GAMES);
    printf("Each round: guess a number between 1 and 100.\n");
    printf("You have %d attempts per round.\n\n", MAX_ATTEMPTS);

    // Loop through each round
    for (round = 0; round < NUM_GAMES; round++) {
        printf("--- Round %d of %d ---\n", round + 1, NUM_GAMES);
        attempts = 0;
        won = 0;

        // Clear the guesses array for this round
        for (i = 0; i < MAX_ATTEMPTS; i++) {
            guesses[i] = 0;
        }

        // Attempts loop
        while (attempts < MAX_ATTEMPTS) {
            printf("Attempt %d/%d - Enter your guess: ",
                   attempts + 1, MAX_ATTEMPTS);

            if (scanf("%d", &guesses[attempts]) != 1) {
                printf("Invalid input. Try again.\n");
                while (getchar() != '\n');
                continue;
            }

            attempts++;

            if (guesses[attempts - 1] == targets[round]) {
                printf("Correct! You guessed it in %d attempts.\n\n", attempts);
                won = 1;
                break;
            } else if (guesses[attempts - 1] < targets[round]) {
                printf("Too low!\n");
            } else {
                printf("Too high!\n");
            }
        }

        // Show the guesses made this round (using array)
        printf("Your guesses were: ");
        for (i = 0; i < attempts; i++) {
            printf("%d ", guesses[i]);
        }
        printf("\n");

        if (!won) {
            printf("Out of attempts! The number was %d.\n\n", targets[round]);
        }
    }

    // Show all secret numbers at the end (using array)
    printf("=== Game Over ===\n");
    printf("The secret numbers were: ");
    for (round = 0; round < NUM_GAMES; round++) {
        printf("%d ", targets[round]);
    }
    printf("\n");

    return 0;
}