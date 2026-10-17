#include <stdio.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

int main() {
    FILE *fin = fopen("ladder.in", "r");
    FILE *fout = fopen("ladder.out", "w");

    if (fin == NULL || fout == NULL) {
        return 1;
    }

    int n;
    fscanf(fin, "%d", &n);

    long long a[105];

    for (int i = 1; i <= n; i++) {
        fscanf(fin, "%lld", &a[i]);
    }

    long long dp[105];

    dp[0] = 0;
    
    dp[1] = a[1];

    for (int i = 2; i <= n; i++) {

        dp[i] = a[i] + max(dp[i - 1], dp[i - 2]);
    }

    fprintf(fout, "%lld\n", dp[n]);

    fclose(fin);
    fclose(fout);

    return 0;
}