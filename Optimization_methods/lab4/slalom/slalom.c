#include <stdio.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

int main() {
    int n;
    scanf("%d", &n);
    
    int a[105][105];
    int dp[105][105];
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            scanf("%d", &a[i][j]);
        }
    }
    
    dp[1][1] = a[1][1];
    
    for (int i = 2; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            if (j == 1) {
                dp[i][j] = a[i][j] + dp[i-1][j];
            } else if (j == i) {
                dp[i][j] = a[i][j] + dp[i-1][j-1];
            } else {
                dp[i][j] = a[i][j] + max(dp[i-1][j], dp[i-1][j-1]);
            }
        }
    }
    
    int answer = dp[n][1];
    for (int j = 2; j <= n; j++) {
        if (dp[n][j] > answer) {
            answer = dp[n][j];
        }
    }
    
    printf("%d\n", answer);
    return 0;
}