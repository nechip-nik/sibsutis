#include <stdio.h>


int main() {
    FILE *fin = fopen("lepus.in", "r");
    FILE *fout = fopen("lepus.out", "w");

    if (fin == NULL || fout == NULL) {
        return 1;
    }

    int n;
    fscanf(fin, "%d", &n);

    char a[1005];

    fscanf(fin, " %s", a + 1); 

    long long dp[1005];

    for (int i = 0; i <= n; i++) {
        dp[i] = -1000000;
    }

    dp[1] = 0;

    for (int i = 2; i <= n; i++) {
        if (a[i] == 'w') {
            continue; 
        }

        long long v1 = (i - 1 >= 1) ? dp[i - 1] : -1000000;
        long long v3 = (i - 3 >= 1) ? dp[i - 3] : -1000000;
        long long v5 = (i - 5 >= 1) ? dp[i - 5] : -1000000;

        // Находим лучший предыдущий путь
        long long best_prev = v1;
        if (v3 > best_prev) best_prev = v3;
        if (v5 > best_prev) best_prev = v5;

        if (best_prev == -1000000) {
            continue;
        }
        int grass = (a[i] == '"') ? 1 : 0;
        dp[i] = best_prev + grass;
    }

    if (dp[n] == -1000000) {
        fprintf(fout, "-1\n");
    } else {
        fprintf(fout, "%lld\n", dp[n]);
    }

    fclose(fin);
    fclose(fout);

    return 0;
}