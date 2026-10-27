#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/* ═══════════════════════════════════════════════════════════
   SINGLY LINKED LIST — Score History
═══════════════════════════════════════════════════════════ */
struct ScoreNode {
    int points;
    int correct;
    char question[64];
    struct ScoreNode *next;
};

struct ScoreNode* scoreCreate(int points, int correct, const char *q) {
    struct ScoreNode *n = malloc(sizeof *n);
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->points = points;
    n->correct = correct;
    snprintf(n->question, sizeof n->question, "%s", q);
    n->next = NULL;
    return n;
}

void scoreAppend(struct ScoreNode **head, struct ScoreNode *node) {
    if (!*head) { *head = node; return; }
    struct ScoreNode *cur = *head;
    while (cur->next) cur = cur->next;
    cur->next = node;
}

void scorePrint(const struct ScoreNode *head) {
    if (!head) { printf("  (no scores yet)\n"); return; }
    int total = 0, correct = 0;
    printf("\n  +-------------------------------------------+\n");
    printf("  |          SCORE HISTORY (Singly LL)        |\n");
    printf("  +-------------------------------------------+\n");
    for (const struct ScoreNode *c = head; c; c = c->next) {
        printf("  |  %-10s  %s  %+3d pts\n",
               c->correct ? "CORRECT" : "WRONG  ",
               c->question, c->points);
        total += c->points;
        if (c->correct) correct++;
    }
    printf("  +-------------------------------------------+\n");
    printf("  |  Total: %+d  |  Accuracy: %d\n", total, correct);
    printf("  +-------------------------------------------+\n");
}

void scoreFree(struct ScoreNode *head) {
    while (head) { struct ScoreNode *t = head; head = head->next; free(t); }
}

/* ═══════════════════════════════════════════════════════════
   DOUBLY LINKED LIST — Question Queue
═══════════════════════════════════════════════════════════ */
struct QNode {
    int a, b, op;
    int answer;
    int answered;
    int playerAns;
    struct QNode *prev, *next;
};

struct QNode* qCreate(int a, int b, int op) {
    struct QNode *n = malloc(sizeof *n);
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->a = a; n->b = b; n->op = op;
    n->answer = (op==0)? a+b : (op==1)? a-b : (op==2)? a*b : a/b;
    n->answered = 0;
    n->playerAns = 0;
    n->prev = n->next = NULL;
    return n;
}

void qPushTail(struct QNode **head, struct QNode *n) {
    if (!*head) { *head = n; return; }
    struct QNode *cur = *head;
    while (cur->next) cur = cur->next;
    cur->next = n;
    n->prev = cur;
}

struct QNode* qStep(struct QNode *cur, int step) {
    (void)step;
    if (!cur) return NULL;
    return step > 0 ? cur->next : cur->prev;
}

void qPrintAll(const struct QNode *head) {
    if (!head) { printf("  (queue empty)\n"); return; }
    printf("\n  +-------------------------------------------+\n");
    printf("  |       QUESTION QUEUE (Doubly LL)         |\n");
    printf("  +-------------------------------------------+\n");
    for (const struct QNode *c = head; c; c = c->next) {
        const char *ops = "+-*/";
        const char *status = c->answered ? (c->playerAns == c->answer ? "Y" : "N") : ".";
        printf("  |  [%s] %2d %c %2d = ?\n", status, c->a, ops[c->op], c->b);
    }
    printf("  +-------------------------------------------+\n");
}

void qFree(struct QNode *head) {
    while (head) { struct QNode *t = head; head = head->next; free(t); }
}

/* ═══════════════════════════════════════════════════════════
   CIRCULAR SINGLY LINKED LIST — Difficulty & Power-Up Ring
═══════════════════════════════════════════════════════════ */
struct RingNode {
    char label[32];
    int value;
    struct RingNode *next;
};

struct RingNode* ringCreate(const char *label, int value) {
    struct RingNode *n = malloc(sizeof *n);
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    snprintf(n->label, sizeof n->label, "%s", label);
    n->value = value;
    n->next = NULL;
    return n;
}

void ringPush(struct RingNode **head, struct RingNode **tail, struct RingNode *n) {
    if (!*head) {
        *head = *tail = n;
        n->next = n;
    } else {
        n->next = *head;
        (*tail)->next = n;
        *tail = n;
    }
}

struct RingNode* ringAdvance(struct RingNode **cur) {
    *cur = (*cur)->next;
    return *cur;
}

void ringPrint(const struct RingNode *head) {
    if (!head) return;
    printf("  Ring: ");
    const struct RingNode *c = head;
    do {
        printf("%s(%d) -> ", c->label, c->value);
        c = c->next;
    } while (c != head);
    printf("(loop)\n");
}

void ringFree(struct RingNode *head) {
    if (!head) return;
    struct RingNode *tail = head;
    while (tail->next != head) tail = tail->next;
    tail->next = NULL;
    while (head) { struct RingNode *t = head; head = head->next; free(t); }
}

/* ═══════════════════════════════════════════════════════════
   GAME ENGINE
═══════════════════════════════════════════════════════════ */

struct QNode* generateQuestion(int diff) {
    int max = 10 * diff;
    if (max < 10) max = 10;
    int a = rand() % max + 1;
    int b = rand() % max + 1;
    int op = rand() % 4;

    if (op == 3) {
        if (b == 0) b = 1;
        a = a * b;
    }
    if (op == 1 && b > a) { int t = a; a = b; b = t; }

    return qCreate(a, b, op);
}

int main(void) {
    srand(time(NULL));

    struct RingNode *diffHead = NULL, *diffTail = NULL;
    struct RingNode *curDiff = NULL;

    ringPush(&diffHead, &diffTail, ringCreate("Easy", 1));
    ringPush(&diffHead, &diffTail, ringCreate("Medium", 3));
    ringPush(&diffHead, &diffTail, ringCreate("Hard", 5));
    curDiff = diffHead;

    struct RingNode *puHead = NULL, *puTail = NULL;
    struct RingNode *curPU = NULL;

    ringPush(&puHead, &puTail, ringCreate("None", 0));
    ringPush(&puHead, &puTail, ringCreate("Double", 2));
    ringPush(&puHead, &puTail, ringCreate("Bonus+5", 5));
    ringPush(&puHead, &puTail, ringCreate("Skip", -1));
    curPU = puHead;

    struct ScoreNode *scoreHead = NULL;
    struct QNode *qHead = NULL;
    struct QNode *qCur = NULL;

    int rounds = 10;
    int round = 0;

    printf("+-----------------------------------------------+\n");
    printf("|        MATH RUSH - Linked List Edition        |\n");
    printf("|  Singly: Scores | Doubly: Queue | Circular: D/P|\n");
    printf("+-----------------------------------------------+\n\n");

    while (round < rounds) {
        round++;
        curDiff = ringAdvance(&curDiff);
        curPU   = ringAdvance(&curPU);

        struct QNode *nq = generateQuestion(curDiff->value);
        qPushTail(&qHead, nq);
        qCur = nq;

        printf("-- Round %d/%d -- [%s] [Power: %s]\n",
               round, rounds, curDiff->label, curPU->label);
        qPrintAll(qHead);

        if (curPU->value == -1) {
            printf("  ** SKIP activated! Moving on...\n\n");
            scoreAppend(&scoreHead, scoreCreate(0, 0, "(skipped)"));
            continue;
        }

        const char *ops = "+-*/";
        printf("  What is %d %c %d ? ", nq->a, ops[nq->op], nq->b);
        int ans;
        scanf("%d", &ans);

        nq->answered = 1;
        nq->playerAns = ans;

        int points = 0;
        char qtext[64];
        snprintf(qtext, sizeof qtext, "%d%c%d=%d", nq->a, ops[nq->op], nq->b, nq->answer);

        if (ans == nq->answer) {
            points = 10 * curDiff->value;
            if (curPU->value == 2) points *= 2;
            if (curPU->value == 5) points += 5;
            printf("  [Y] Correct! +%d pts\n", points);
        } else {
            points = -5;
            printf("  [N] Wrong! Answer was %d. %+d pts\n", nq->answer, points);
        }
        scoreAppend(&scoreHead, scoreCreate(points, ans == nq->answer, qtext));
        printf("\n");
    }

    scorePrint(scoreHead);
    printf("\n  Difficulty cycle: ");
    ringPrint(diffHead);
    printf("  Power-up cycle:   ");
    ringPrint(puHead);
    printf("\n  Game over! Thanks for playing.\n");

    scoreFree(scoreHead);
    qFree(qHead);
    ringFree(diffHead);
    ringFree(puHead);

    return 0;
}   