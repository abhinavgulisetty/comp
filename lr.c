#include <stdio.h>
#include <string.h>

#define MAXP 50
#define MAXA 50
#define MAXL 100

typedef struct {
    char lhs;
    char rhs[MAXA][MAXL];
    int count;
} Production;

Production G[MAXP];
int n = 0;
char nextNT = 'Z';   

char getNewNT() {
    return nextNT--;
}

void substitute(int i, int j) {
    char temp[MAXA][MAXL];
    int t = 0;

    for (int k = 0; k < G[i].count; k++) {

        if (G[i].rhs[k][0] == G[j].lhs) {

            char gamma[MAXL];
            strcpy(gamma, G[i].rhs[k] + 1);

            for (int m = 0; m < G[j].count; m++) {

                if (strcmp(G[j].rhs[m], "#") == 0)
                    sprintf(temp[t++], "%s", gamma);
                else
                    sprintf(temp[t++], "%s%s", G[j].rhs[m], gamma);
            }
        }

        else {
            strcpy(temp[t++], G[i].rhs[k]);
        }
    }

    G[i].count = t;

    for (int k = 0; k < t; k++)
        strcpy(G[i].rhs[k], temp[k]);
}

void remove_direct(int i) {

    char alpha[MAXA][MAXL];
    char beta[MAXA][MAXL];

    int a = 0, b = 0;

    for (int j = 0; j < G[i].count; j++) {

        if (G[i].rhs[j][0] == G[i].lhs)
            strcpy(alpha[a++], G[i].rhs[j] + 1);
        else
            strcpy(beta[b++], G[i].rhs[j]);
    }

    if (a == 0) return;

    char newNT = getNewNT();

    Production newProd;
    newProd.lhs = newNT;
    newProd.count = 0;

    G[i].count = 0;

    for (int j = 0; j < b; j++) {

        if (strcmp(beta[j], "#") == 0)
            sprintf(G[i].rhs[G[i].count++], "%c", newNT);
        else
            sprintf(G[i].rhs[G[i].count++], "%s%c", beta[j], newNT);
    }

    for (int j = 0; j < a; j++)
        sprintf(newProd.rhs[newProd.count++], "%s%c", alpha[j], newNT);

    strcpy(newProd.rhs[newProd.count++], "#");

    G[n++] = newProd;
}

int main() {

    char line[MAXL];

    printf("Enter productions\n");

    while (1) {

        printf("P[%d]: ", n + 1);

        fgets(line, MAXL, stdin);
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "end") == 0)
            break;

        G[n].lhs = line[0];
        G[n].count = 0;

        char *rhs = strchr(line, '=');
        rhs++;

        char temp[MAXL];
        strcpy(temp, rhs);

        char *tok = strtok(temp, "|");

        while (tok) {
            strcpy(G[n].rhs[G[n].count++], tok);
            tok = strtok(NULL, "|");
        }

        n++;
    }

    for (int i = 0; i < n; i++) {

        for (int j = 0; j < i; j++)
            substitute(i, j);

        remove_direct(i);
    }

    printf("\nAfter Removing Left Recursion:\n");

    for (int i = 0; i < n; i++) {

        printf("%c -> ", G[i].lhs);

        for (int j = 0; j < G[i].count; j++) {

            printf("%s", G[i].rhs[j]);

            if (j < G[i].count - 1)
                printf(" | ");
        }

        printf("\n");
    }

    return 0;
}