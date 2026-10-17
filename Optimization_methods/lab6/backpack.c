#include <stdio.h>
#include <stdlib.h>

void bounded_knapsack(int W, const int *n, const int *c, const int *m, int k)
{
    int total = 0;
    for (int i = 0; i < k; ++i) total += m[i];

    int *wn = malloc(total * sizeof(int));
    int *cv = malloc(total * sizeof(int));
    int *tp = malloc(total * sizeof(int));
    if (!wn || !cv || !tp) { fprintf(stderr, "oom\n"); exit(1); }

    int idx = 0;
    for (int i = 0; i < k; ++i)
        for (int t = 0; t < m[i]; ++t) {
            wn[idx] = n[i]; cv[idx] = c[i]; tp[idx] = i; idx++;
        }

    int *d = calloc(W + 1, sizeof(int));
    int *last = malloc((W + 1) * sizeof(int));
    if (!d || !last) { fprintf(stderr, "oom\n"); exit(1); }
    for (int x = 0; x <= W; ++x) last[x] = -1;

    for (int i = 0; i < total; ++i)
        for (int x = W; x >= wn[i]; --x) {
            int cand = d[x - wn[i]] + cv[i];
            if (cand > d[x]) { d[x] = cand; last[x] = i; }
        }

    int *used = calloc(k, sizeof(int));
    int rem = W;
    while (rem > 0 && last[rem] != -1) {
        int i = last[rem]; used[tp[i]]++; rem -= wn[i];
    }

    printf("=== РЕЗУЛЬТАТ ===\n");
    printf("Максимальная стоимость: %d\n\n", d[W]);
    int tw = 0, tv = 0;
    for (int i = 0; i < k; ++i)
        if (used[i] > 0) {
            int w_i = used[i] * n[i], v_i = used[i] * c[i];
            printf("  Тип %d: %d шт. (вес: %d, стоимость: %d)\n", i+1, used[i], w_i, v_i);
            tw += w_i; tv += v_i;
        }
    printf("\nОбщий вес: %d / %d\n", tw, W);
    printf("Общая стоимость: %d\n", tv);

    free(wn); free(cv); free(tp);
    free(d); free(last); free(used);
}

int main(void)
{
    int n[] = {3, 5, 7};
    int c[] = {8, 14, 20};
    int m[] = {2, 1, 3};
    int k = 3;
    int W = 5000000;
    bounded_knapsack(W, n, c, m, k);
    return 0;
}
