#include <stdio.h>
#include <limits.h>

#define N 8

int min(int a, int b, int c) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

int main() {
    FILE *fin = fopen("king2.in", "r");
    FILE *fout = fopen("king2.out", "w");

    if (fin == NULL || fout == NULL) {
        printf("Ошибка открытия файлов.\n");
        return 1;
    }

    int cost[N][N];
    int dp[N][N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fscanf(fin, "%d", &cost[i][j]);
        }
    }

    dp[7][0] = cost[7][0]; 

    for (int i = N - 2; i >= 0; i--) {
        dp[i][0] = dp[i+1][0] + cost[i][0];
    }

    for (int j = 1; j < N; j++) {
        dp[N-1][j] = dp[N-1][j-1] + cost[N-1][j];
    }

    for (int i = N - 2; i >= 0; i--) {
        for (int j = 1; j < N; j++) {
            int best_prev = min(dp[i+1][j], dp[i][j-1], dp[i+1][j-1]);
            dp[i][j] = cost[i][j] + best_prev;
        }
    }

    fprintf(fout, "%d\n", dp[0][N-1]);

    fclose(fin);
    fclose(fout);

    return 0;
}