#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_ROUNDS 10

struct Question {
    int a, b, op;
    int answer;
    int answered;
    int playerAns;
};

struct Score {
    int points;
    int correct;
    char text[32];
};

int main(void) {
    srand(time(NULL));

    /* "Circular" rings are just arrays + index modulo */
    const char *diffNames[] = {"Easy", "Medium", "Hard"};
    int diffVals[] = {1, 3, 5};
    int diffCount = 3;

    const char *puNames[] = {"None", "Double", "Bonus+5", "Skip"};
    int puVals[] = {0, 2, 5, -1};
    int puCount = 4;

    /* "Doubly" queue → fixed array + count */
    struct Question queue[MAX_ROUNDS];
    int qCount = 0;

    /* "Singly" history → fixed array + count */
    struct Score scores[MAX_ROUNDS];
    int sCount = 0;

    int diffIdx = 0, puIdx = 0;

    printf("+-----------------------------------------------+\n");
    printf("|        MATH RUSH - Array Edition            |\n");
    printf("+-----------------------------------------------+\n\n");

    for (int round = 0; round < MAX_ROUNDS; round++) {
        /* Advance rings (O(1), just index math) */
        diffIdx = (diffIdx + 1) % diffCount;
        puIdx   = (puIdx + 1) % puCount;

        /* Generate question */
        int max = 10 * diffVals[diffIdx];
        if (max < 10) max = 10;
        int a = rand() % max + 1;
        int b = rand() % max + 1;
        int op = rand() % 4;
        if (op == 3) { if (b == 0) b = 1; a = a * b; }
        if (op == 1 && b > a) { int t = a; a = b; b = t; }

        int answer = (op==0)? a+b : (op==1)? a-b : (op==2)? a*b : a/b;

        /* Enqueue (just write to next slot) */
        queue[qCount].a = a;
        queue[qCount].b = b;
        queue[qCount].op = op;
        queue[qCount].answer = answer;
        queue[qCount].answered = 0;
        queue[qCount].playerAns = 0;

        printf("-- Round %d/%d -- [%s] [Power: %s]\n",
               round + 1, MAX_ROUNDS, diffNames[diffIdx], puNames[puIdx]);

        /* Print queue */
        printf("  Queue:");
        for (int i = 0; i <= qCount; i++) {
            const char *ops = "+-*/";
            char st = queue[i].answered ? (queue[i].playerAns == queue[i].answer ? 'Y' : 'N') : '.';
            printf(" [%c] %d%c%d", st, queue[i].a, ops[queue[i].op], queue[i].b);
        }
        printf("\n");

        /* Handle skip */
        if (puVals[puIdx] == -1) {
            printf("  ** SKIP! Moving on...\n\n");
            scores[sCount].points = 0;
            scores[sCount].correct = 0;
            strcpy(scores[sCount].text, "(skipped)");
            sCount++;
            qCount++;
            continue;
        }

        /* Ask player */
        const char *ops = "+-*/";
        printf("  What is %d %c %d ? ", a, ops[op], b);
        int ans;
        scanf("%d", &ans);

        /* Score */
        int points = 0;
        int correct = (ans == answer);
        if (correct) {
            points = 10 * diffVals[diffIdx];
            if (puVals[puIdx] == 2) points *= 2;
            if (puVals[puIdx] == 5) points += 5;
            printf("  [Y] Correct! +%d pts\n", points);
        } else {
            points = -5;
            printf("  [N] Wrong! Answer was %d. %+d pts\n", answer, points);
        }

        queue[qCount].answered = 1;
        queue[qCount].playerAns = ans;
        qCount++;

        scores[sCount].points = points;
        scores[sCount].correct = correct;
        snprintf(scores[sCount].text, sizeof scores[sCount].text, "%d%c%d=%d", a, ops[op], b, answer);
        sCount++;
        printf("\n");
    }

    /* Final report */
    printf("+-------------------------------------------+\n");
    printf("|          SCORE HISTORY                      |\n");
    printf("+-------------------------------------------+\n");
    int total = 0, correctCount = 0;
    for (int i = 0; i < sCount; i++) {
        printf("  %-10s  %-20s  %+3d pts\n",
               scores[i].correct ? "CORRECT" : "WRONG",
               scores[i].text, scores[i].points);
        total += scores[i].points;
        if (scores[i].correct) correctCount++;
    }
    printf("+-------------------------------------------+\n");
    printf("  Total: %+d  |  Accuracy: %d/%d\n", total, correctCount, sCount);
    printf("\n  Game over! Thanks for playing.\n");

    return 0;
}   