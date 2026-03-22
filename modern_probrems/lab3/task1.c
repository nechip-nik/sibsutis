#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int** create_F_matrix(int* truth_table) {
    int** F = (int**)malloc(8 * sizeof(int*));
    for (int i = 0; i < 8; i++) {
        F[i] = (int*)calloc(8, sizeof(int));
    }

    for (int x1 = 0; x1 < 2; x1++) {
        for (int x2 = 0; x2 < 2; x2++) {
            for (int y = 0; y < 2; y++) {
                int i = (x1 << 2) | (x2 << 1) | y;
                int f = truth_table[(x1 << 1) | x2];
                int new_y = y ^ f;
                int j = (x1 << 2) | (x2 << 1) | new_y;
                F[i][j] = 1;
            }
        }
    }
    return F;
}

void apply_F(int** F, int* state, int* result) {
    for (int i = 0; i < 8; i++) {
        result[i] = 0;
        for (int j = 0; j < 8; j++) {
            result[i] += F[i][j] * state[j];
        }
    }
}

void print_state(int* state) {
    printf("|");
    for (int i = 0; i < 8; i++) {
        if (state[i] != 0) {
            int x1 = (i >> 2) & 1;
            int x2 = (i >> 1) & 1;
            int y = i & 1;
            printf("%d%d%d", x1, x2, y);
        }
    }
    printf("⟩\n");
}

int main() {
    int truth_table[4];
    int x1, x2, y;
    
    printf("Введите таблицу истинности булевой функции f(x1, x2):\n");
    printf("f(00) = "); scanf("%d", &truth_table[0]);
    printf("f(01) = "); scanf("%d", &truth_table[1]);
    printf("f(10) = "); scanf("%d", &truth_table[2]);
    printf("f(11) = "); scanf("%d", &truth_table[3]);
    
    for (int i = 0; i < 4; i++) {
        if (truth_table[i] != 0 && truth_table[i] != 1) {
            printf("Ошибка! Значения должны быть 0 или 1.\n");
            return 1;
        }
    }
    
    printf("\nТаблица истинности: f(00)=%d, f(01)=%d, f(10)=%d, f(11)=%d\n", 
           truth_table[0], truth_table[1], truth_table[2], truth_table[3]);
    
    printf("\nВведите значения переменных для вычисления:\n");
    printf("x1 (0 или 1): "); scanf("%d", &x1);
    printf("x2 (0 или 1): "); scanf("%d", &x2);
    printf("y  (0 или 1): "); scanf("%d", &y);
    
    if ((x1 != 0 && x1 != 1) || (x2 != 0 && x2 != 1) || (y != 0 && y != 1)) {
        printf("Ошибка! Значения должны быть 0 или 1.\n");
        return 1;
    }

    int** F = create_F_matrix(truth_table);
    
    printf("\nМатрица преобразования F (8x8):\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d ", F[i][j]);
        }
        printf("\n");
    }
    
    int input_state[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int input_index = (x1 << 2) | (x2 << 1) | y;
    input_state[input_index] = 1;
    
    printf("\nВходное состояние: |%d%d%d⟩\n", x1, x2, y);
    
    int output_state[8];
    apply_F(F, input_state, output_state);
    
    printf("Выходное состояние: ");
    print_state(output_state);
    
    int output_index = -1;
    for (int i = 0; i < 8; i++) {
        if (output_state[i] == 1) {
            output_index = i;
            break;
        }
    }
    
    if (output_index != -1) {
        int out_x1 = (output_index >> 2) & 1;
        int out_x2 = (output_index >> 1) & 1;
        int out_y = output_index & 1;
        
        printf("\nРезультат вычисления:\n");
        printf("f(%d%d) = %d\n", x1, x2, truth_table[(x1 << 1) | x2]);
        printf("y ⊕ f = %d ⊕ %d = %d\n", y, truth_table[(x1 << 1) | x2], out_y);
        printf("Проверка: x1=%d, x2=%d (не изменились) ✓\n", out_x1, out_x2);
    }
    
    for (int i = 0; i < 8; i++) {
        free(F[i]);
    }
    free(F);
    
    return 0;
}