#include <stdio.h>

int main() {
    FILE *fin = fopen("knight.in", "r");
    FILE *fout = fopen("knight.out", "w");

    int N, M;
    fscanf(fin, "%d %d", &N, &M);
    long long dp[55][55] = {0};

    dp[1][1] = 1;

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= M; j++) {
            
            if (i == 1 && j == 1) continue;

            long long ways = 0;

            // Ход 1: конь пришел с клетки (i-2, j-1) — на 2 вниз, 1 вправо
            if (i - 2 >= 1 && j - 1 >= 1) {
                ways += dp[i-2][j-1];
            }

            // Ход 2: конь пришел с клетки (i-1, j-2) — на 1 вниз, 2 вправо
            if (i - 1 >= 1 && j - 2 >= 1) {
                ways += dp[i-1][j-2];
            }

            dp[i][j] = ways;
        }
    }

    fprintf(fout, "%lld\n", dp[N][M]);

    fclose(fin);
    fclose(fout);
    return 0;
}