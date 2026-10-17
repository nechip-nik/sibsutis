#include <stdio.h>
#include <stdlib.h>

void unbounded_knapsack(int W, const int *n, const int *c, int k)
{
    int *d = calloc(W + 1, sizeof(int));      // максимальная стоимость для вместимости x
    int *last_item = malloc((W + 1) * sizeof(int)); // индекс последней добавленной вещи
    
    if (!d || !last_item) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }

    for (int x = 0; x <= W; ++x) {
        int best = 0;
        int best_item = -1;
        
        for (int i = 0; i < k; ++i) {
            if (n[i] <= x) {
                int cand = d[x - n[i]] + c[i];
                if (cand > best) {
                    best = cand;
                    best_item = i;
                }
            }
        }
        
        d[x] = best;
        last_item[x] = best_item;
        
        printf("d(%d) = %d", x, best);
        
        if (best_item == -1) {
            printf("  (пусто)");
        } else {
            printf("  (вещи: ");
            int rem = x;
            int first = 1;
            while (rem > 0 && last_item[rem] != -1) {
                int idx = last_item[rem];
                if (!first) printf(", ");
                printf("тип %d", idx + 1);
                first = 0;
                rem -= n[idx];
            }
            printf(")");
        }
        printf("\n");
    }

    free(d);
    free(last_item);
}

int main(void)
{
    int n[] = {3, 5, 7};   // веса
    int c[] = {8, 14, 20}; // стоимости
    int k = sizeof(n) / sizeof(n[0]);
    int W = 18;

    unbounded_knapsack(W, n, c, k);

    return 0;
}