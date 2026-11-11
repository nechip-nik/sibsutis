#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAXN 1000

static int d[MAXN + 1][MAXN + 1];
static int prv[MAXN + 1][MAXN + 1];
static int blocked[MAXN + 1];

int frog(int n, const int *cells, int cells_len, int *jumps, int *jumps_len)
{
    for (int i = 0; i <= n; i++) blocked[i] = 0;
    for (int i = 0; i < cells_len; i++)
        if (cells[i] >= 0 && cells[i] <= n) blocked[cells[i]] = 1;

    for (int x = 0; x <= n; x++)
        for (int dl = 0; dl <= n; dl++) {
            d[x][dl] = INT_MAX;
            prv[x][dl] = -1;
        }

    d[0][0] = 0;

    for (int x = 0; x < n; x++) {
        for (int dl = 0; dl <= n; dl++) {
            if (d[x][dl] == INT_MAX) continue;

            int minJump = (dl == 0) ? 1 : (dl - 1 > 1 ? dl - 1 : 1);
            int maxJump = (dl == 0) ? 1 : dl + 1;

            for (int nextDl = minJump; nextDl <= maxJump; nextDl++) {
                int nextX = x + nextDl;
                if (nextX > n || blocked[nextX]) continue;

                if (d[x][dl] + 1 < d[nextX][nextDl]) {
                    d[nextX][nextDl] = d[x][dl] + 1;
                    prv[nextX][nextDl] = dl;
                }
            }
        }
    }

    int result = INT_MAX, lastJump = -1;
    for (int dl = 1; dl <= n; dl++) {
        if (d[n][dl] < result) {
            result = d[n][dl];
            lastJump = dl;
        }
    }

    *jumps_len = 0;
    if (lastJump == -1) return -1;

    int pos = n;
    int buf[MAXN], len = 0;
    while (pos > 0 && lastJump > 0) {
        buf[len++] = lastJump;
        int prevDl = prv[pos][lastJump];
        pos -= lastJump;
        lastJump = prevDl;
    }
    for (int i = 0; i < len; i++)
        jumps[i] = buf[len - 1 - i];

    *jumps_len = len;
    return result;
}

void print_dp_table(int n)
{
    printf("=== ТАБЛИЦА ДИНАМИКИ ===\n");


    printf("dl\\x ");
    for (int x = 0; x <= n; x++) printf("%4d", x);
    printf("\n");

    printf("-----");
    for (int x = 0; x <= n; x++) printf("----");
    printf("\n");

    for (int dl = 0; dl <= n; dl++) {
        printf("%4d ", dl);
        for (int x = 0; x <= n; x++) {
            if (blocked[x])               printf("   X");
            else if (d[x][dl] == INT_MAX) printf("   .");
            else                          printf("%4d", d[x][dl]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(void)
{
    int n = 10;
    int cells[] = { 4, 5, 9 };
    int cells_len = sizeof(cells) / sizeof(cells[0]);

    int jumps[MAXN];
    int jumps_len = 0;

    int steps = frog(n, cells, cells_len, jumps, &jumps_len);

    printf("=== ПОЛЕ ===\n");
    printf("Камни: ");
    for (int i = 0; i < cells_len; i++) {
        if (i) printf(", ");
        printf("%d", cells[i]);
    }
    printf("\n\n");

    printf("Клетка: ");
    for (int i = 0; i <= n; i++) printf("%2d ", i);
    printf("\n");

    printf("Поле:   ");
    for (int i = 0; i <= n; i++) {
        int stone = 0;
        for (int j = 0; j < cells_len; j++)
            if (cells[j] == i) { stone = 1; break; }
        printf("%2d ", stone ? 1 : 0);
    }
    printf("\n\n");

    printf("=== РЕЗУЛЬТАТ ===\n");
    if (steps == -1) {
        printf("Путь не найден\n\n");
        print_dp_table(n);
        return 0;
    }

    printf("Минимальное число прыжков: %d\n", steps);
    printf("Длины прыжков: ");
    for (int i = 0; i < jumps_len; i++) {
        if (i) printf(" ");
        printf("%d", jumps[i]);
    }
    printf("\n\n");

    print_dp_table(n);

    return 0;
}