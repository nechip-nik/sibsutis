#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bounded_knapsack(int W, const int *n, const int *c, const int *m, int k)
{
    int *d = calloc(W + 1, sizeof(int));
    if (!d) { fprintf(stderr, "oom d\n"); exit(1); }

    int **lay = malloc((k + 1) * sizeof(int *));
    if (!lay) { fprintf(stderr, "oom lay\n"); exit(1); }
    lay[0] = malloc((W + 1) * sizeof(int));
    memcpy(lay[0], d, (W + 1) * sizeof(int));

    for (int i = 0; i < k; ++i) {
        lay[i + 1] = malloc((W + 1) * sizeof(int));
        if (!lay[i + 1]) { fprintf(stderr, "oom lay[%d]\n", i + 1); exit(1); }

        memcpy(lay[i + 1], lay[i], (W + 1) * sizeof(int));

        for (int cnt = 1; cnt <= m[i]; ++cnt) {
            int wi = cnt * n[i];
            int vi = cnt * c[i];
            for (int x = W; x >= wi; --x) {
                int cand = lay[i][x - wi] + vi;
                if (cand > lay[i + 1][x])
                    lay[i + 1][x] = cand;
            }
        }
    }
    int *used = calloc(k, sizeof(int));
    int x = W;
    for (int i = k - 1; i >= 0; --i) {
        if (lay[i + 1][x] == lay[i][x]) {
            used[i] = 0;
            continue;
        }
        int found = 0;
        for (int cnt = m[i]; cnt >= 1; --cnt) {
            int wi = cnt * n[i];
            int vi = cnt * c[i];
            if (wi <= x && lay[i][x - wi] + vi == lay[i + 1][x]) {
                used[i] = cnt;
                x -= wi;
                found = 1;
                break;
            }
        }
        if (!found) used[i] = 0;
    }

    int tw = 0, tv = 0;
    printf("Взяли в рюкзак:\n");
    for (int i = 0; i < k; ++i) {
        if (used[i] > 0) {
            int w_i = used[i] * n[i], v_i = used[i] * c[i];
            printf("  Тип %d: %d шт. (вес: %d, стоимость: %d)\n", i + 1, used[i], w_i, v_i);
            tw += w_i; tv += v_i;
        }
    }
    printf("\nОбщий вес: %d / %d\n", tw, W);
    printf("Общая стоимость: %d\n", tv);

    for (int i = 0; i <= k; ++i) free(lay[i]);
    free(lay); free(d); free(used);
}

int main(void)
{
    int n[] = {3, 5, 7};
    int c[] = {8, 14, 20};
    int m[] = {2, 1, 3};
    int k = 3;
    int W = 16;

    bounded_knapsack(W, n, c, m, k);
    return 0;
}