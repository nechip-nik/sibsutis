#include <stdio.h>
#include <math.h>

#define N 8

typedef struct {
    double real[N];
} QuantumState;

void initState(QuantumState *qs) {
    for (int i = 0; i < N; i++) {
        qs->real[i] = 0.0;
    }
    qs->real[0] = 1.0;
}

void applyH(QuantumState *qs, int qubit) {
    int step = 1 << qubit;
    int length = step << 1;
    double inv_sqrt2 = 1.0 / sqrt(2.0);

    for (int i = 0; i < N; i += length) {
        for (int j = i; j < i + step; j++) {
            int k = j + step;
            double a_re = qs->real[j];
            double b_re = qs->real[k];
            qs->real[j] = inv_sqrt2 * (a_re + b_re);
            qs->real[k] = inv_sqrt2 * (a_re - b_re);
        }
    }
}

int f(int x1, int x2, int function_type) {
    switch (function_type) {
        case 0: return 0; 
        case 1: return 1; 
        case 2: return x1 & x2; 
        case 3: return x1 | x2; 
        case 4: return x1 ^ x2; 
        case 5: return (x1 & x2) ^ 1; 
        case 6: return (x1 == x2) ? 0 : 1;
        case 7: return (x1 == 0 && x2 == 0) ? 1 : 0;
        case 8: return (x1 == 1 && x2 == 1) ? 0 : 1;
        default: return 0;
    }
}


void checkBalance(int function_type) {
    int count[2] = {0, 0};
    printf("Значения функции f(x1,x2):\n");
    for (int x1 = 0; x1 <= 1; x1++) {
        for (int x2 = 0; x2 <= 1; x2++) {
            int fx = f(x1, x2, function_type);
            printf("  f(%d,%d) = %d\n", x1, x2, fx);
            count[fx]++;
        }
    }
    printf("  Нулей: %d, Единиц: %d\n", count[0], count[1]);
    if (count[0] == 4) printf("  → КОНСТАНТНАЯ (все нули)\n");
    else if (count[1] == 4) printf("  → КОНСТАНТНАЯ (все единицы)\n");
    else if (count[0] == 2 && count[1] == 2) printf("  → СБАЛАНСИРОВАННАЯ\n");
    else printf("  → НИ константная, НИ сбалансированная\n");
}


void applyOracle(QuantumState *qs, int function_type) {
    double temp[N];
    for (int i = 0; i < N; i++) {
        temp[i] = qs->real[i];
        qs->real[i] = 0.0;
    }
    
    for (int i = 0; i < N; i++) {
        if (fabs(temp[i]) > 1e-10) {
            int x1 = (i >> 2) & 1;
            int x2 = (i >> 1) & 1;
            int y = i & 1;
            int fx = f(x1, x2, function_type);
            int new_y = y ^ fx;
            int new_idx = (x1 << 2) | (x2 << 1) | new_y;
            qs->real[new_idx] += temp[i];
        }
    }
}


void printState(QuantumState *qs) {
    int printed = 0;
    for (int i = 0; i < N; i++) {
        if (fabs(qs->real[i]) > 1e-10) {
            printf("  %.3f |%d%d%d>\n", qs->real[i], (i>>2)&1, (i>>1)&1, i&1);
            printed++;
        }
    }
    if (printed == 0) {
        printf("  0.000 |000>\n");
    }
}


void deutschJozsa(int function_type) {
    QuantumState qs;
    initState(&qs);

    printf("\nФункция типа %d:\n", function_type);
    switch(function_type) {
        case 0: printf("  Константа 0\n"); break;
        case 1: printf("  Константа 1\n"); break;
        case 2: printf("  И (x1&x2)\n"); break;
        case 3: printf("  ИЛИ (x1|x2)\n"); break;
        case 4: printf("  XOR (x1^x2)\n"); break;
        case 5: printf("  НЕ-И\n"); break;
        case 6: printf("  неравенство (x1≠x2)\n"); break;
        case 7: printf("  только f(00)=1\n"); break;
        case 8: printf("  все кроме f(11)=0\n"); break;
    }
    
    checkBalance(function_type);
    

    qs.real[0] = 0.0;
    qs.real[1] = 1.0;
    

    applyH(&qs, 2);
    applyH(&qs, 1);
    applyH(&qs, 0);
    

    applyOracle(&qs, function_type);
    

    applyH(&qs, 2);
    applyH(&qs, 1);
    

    double prob00 = 0.0;
    for (int i = 0; i < 2; i++) {
        prob00 += qs.real[i] * qs.real[i];
    }
    
    printf("  Финальное состояние:\n");
    printState(&qs);
    printf("  Вероятность |00⟩ на первых двух кубитах: %.3f\n", prob00);
    
    if (prob00 > 0.99) {
        printf("  → Функция КОНСТАНТНАЯ\n");
    } else if (prob00 < 0.01) {
        printf("  → Функция СБАЛАНСИРОВАННАЯ\n");
    } else {
        printf("  → Функция НИ константная, НИ сбалансированная\n");
    }
}

int main() {
    printf("АЛГОРИТМ ДОЙЧА–ЁЖИ ДЛЯ ФУНКЦИЙ ОТ ДВУХ ПЕРЕМЕННЫХ\n");
    printf("================================================\n");
    
    for (int func = 0; func <= 8; func++) {
        deutschJozsa(func);
    }
    
    return 0;
}