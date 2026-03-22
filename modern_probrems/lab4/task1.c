#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
    int n_qubits;           
    int n_states;           
    double *amplitudes;     
    int *phases;            
} QuantumState;


QuantumState* init_state(int n_qubits) {
    QuantumState *qs = (QuantumState*)malloc(sizeof(QuantumState));
    qs->n_qubits = n_qubits;
    qs->n_states = 1 << n_qubits; 
    
    qs->amplitudes = (double*)calloc(qs->n_states, sizeof(double));
    qs->phases = (int*)malloc(qs->n_states * sizeof(int));
    
    qs->amplitudes[0] = 1.0;
    for (int i = 0; i < qs->n_states; i++) {
        qs->phases[i] = 1;  
    }
    
    return qs;
}

void free_state(QuantumState *qs) {
    if (qs != NULL) {
        if (qs->amplitudes != NULL) free(qs->amplitudes);
        if (qs->phases != NULL) free(qs->phases);
        free(qs);
    }
}

double get_state_value(QuantumState *qs, int index) {
    if (index < 0 || index >= qs->n_states) return 0.0;
    return qs->amplitudes[index] * qs->phases[index];
}

void apply_hadamard(QuantumState *qs, int q) {
    if (q < 0 || q >= qs->n_qubits) return;
    
    int shift = 1 << q;  
    int block_size = 1 << (q + 1);  
    double *new_amplitudes = (double*)calloc(qs->n_states, sizeof(double));
    int *new_phases = (int*)malloc(qs->n_states * sizeof(int));
    
    for (int i = 0; i < qs->n_states; i++) {
        new_phases[i] = 1;
    }
    
    for (int start = 0; start < qs->n_states; start += block_size) {
        for (int i = start; i < start + shift; i++) {
            double a = get_state_value(qs, i);
            double b = get_state_value(qs, i + shift);
            
            double new_a = (a + b) / sqrt(2.0);
            double new_b = (a - b) / sqrt(2.0);

            if (new_a >= 0) {
                new_amplitudes[i] = new_a;
                new_phases[i] = 1;
            } else {
                new_amplitudes[i] = -new_a;
                new_phases[i] = -1;
            }
            
            if (new_b >= 0) {
                new_amplitudes[i + shift] = new_b;
                new_phases[i + shift] = 1;
            } else {
                new_amplitudes[i + shift] = -new_b;
                new_phases[i + shift] = -1;
            }
        }
    }
    
    memcpy(qs->amplitudes, new_amplitudes, qs->n_states * sizeof(double));
    memcpy(qs->phases, new_phases, qs->n_states * sizeof(int));
    
    free(new_amplitudes);
    free(new_phases);
}

void print_state(QuantumState *qs, const char *label) {
    printf("%s:\n", label);
    int printed = 0;
    
    for (int i = 0; i < qs->n_states; i++) {
        double value = get_state_value(qs, i);
        if (fabs(value) > 1e-6) {  
            printf("  |");
            for (int q = qs->n_qubits - 1; q >= 0; q--) {
                printf("%d", (i >> q) & 1);
            }
            printf("⟩: ");
            
            if (value >= 0) {
                printf("+%.3f", value);
            } else {
                printf("-%.3f", -value);
            }
            printf("\n");
            printed++;
        }
    }
    
    if (printed == 0) {
        printf("  (все амплитуды ~0)\n");
    }
    printf("\n");
}

void apply_and_oracle(QuantumState *qs) {

    double *new_amplitudes = (double*)calloc(qs->n_states, sizeof(double));
    int *new_phases = (int*)malloc(qs->n_states * sizeof(int));
    
    for (int i = 0; i < qs->n_states; i++) {
        int x1 = (i >> 2) & 1;  
        int x2 = (i >> 1) & 1;  
        int y = i & 1;           
        int fx = (x1 & x2);  
        int new_y = y ^ fx;
        int new_index = (x1 << 2) | (x2 << 1) | new_y;
        
        new_amplitudes[new_index] = qs->amplitudes[i];
        new_phases[new_index] = qs->phases[i];
    }
    
    memcpy(qs->amplitudes, new_amplitudes, qs->n_states * sizeof(double));
    memcpy(qs->phases, new_phases, qs->n_states * sizeof(int));
    
    free(new_amplitudes);
    free(new_phases);
}

void apply_boolean_oracle(QuantumState *qs, int *truth_table) {
    
    double *new_amplitudes = (double*)calloc(qs->n_states, sizeof(double));
    int *new_phases = (int*)malloc(qs->n_states * sizeof(int));
    
    for (int i = 0; i < qs->n_states; i++) {
        int x1 = (i >> 2) & 1;
        int x2 = (i >> 1) & 1;
        int y = i & 1;
        
        int index = (x1 << 1) | x2;  
        int fx = truth_table[index];
        
        int new_y = y ^ fx;
        int new_index = (x1 << 2) | (x2 << 1) | new_y;
        
        new_amplitudes[new_index] = qs->amplitudes[i];
        new_phases[new_index] = qs->phases[i];
    }
    
    memcpy(qs->amplitudes, new_amplitudes, qs->n_states * sizeof(double));
    memcpy(qs->phases, new_phases, qs->n_states * sizeof(int));
    
    free(new_amplitudes);
    free(new_phases);
}

void apply_arithmetic_oracle(QuantumState *qs, int a, int m) {

    int n = 2;  
    int r = 2;  
    
    double *new_amplitudes = (double*)calloc(qs->n_states, sizeof(double));
    int *new_phases = (int*)malloc(qs->n_states * sizeof(int));
    
    for (int i = 0; i < qs->n_states; i++) {
        int x = (i >> r) & ((1 << n) - 1);  
        int y = i & ((1 << r) - 1);         
        
        int fx = 1;
        for (int j = 0; j < x; j++) {
            fx = (fx * a) % m;
        }
        fx = fx % m;
        
        int new_y = y ^ fx;
        int new_index = (x << r) | new_y;
        
        new_amplitudes[new_index] = qs->amplitudes[i];
        new_phases[new_index] = qs->phases[i];
    }
    
    memcpy(qs->amplitudes, new_amplitudes, qs->n_states * sizeof(double));
    memcpy(qs->phases, new_phases, qs->n_states * sizeof(int));
    
    free(new_amplitudes);
    free(new_phases);
}

void quantum_parallelism_boolean() {
    printf("========================================\n");
    printf("1. КВАНТОВЫЙ ПАРАЛЛЕЛИЗМ ДЛЯ БУЛЕВОЙ ФУНКЦИИ AND\n");
    printf("========================================\n\n");
    
    QuantumState *qs = init_state(3);
    
    printf("Шаг 0: Инициализация\n");
    print_state(qs, "Начальное состояние |000>");
    
    printf("Шаг 1: Применяем преобразование Адамара к x1 и x2\n");
    apply_hadamard(qs, 2);  
    apply_hadamard(qs, 1);  
    print_state(qs, "После H(x1) и H(x2)");
    
    printf("Шаг 2: Применяем для функции AND\n");
    apply_and_oracle(qs);
    print_state(qs, "После применения U_f (AND)");
    
    printf("Анализ результата:\n");
    printf("Состояние содержит суперпозицию всех значений функции AND:\n");
    
    for (int i = 0; i < 8; i++) {
        double val = get_state_value(qs, i);
        if (fabs(val) > 1e-6) {
            int x1 = (i >> 2) & 1;
            int x2 = (i >> 1) & 1;
            int y = i & 1;
            printf("  f(%d%d) = %d: амплитуда ", x1, x2, y);
            if (val >= 0) {
                printf("+%.3f\n", val);
            } else {
                printf("-%.3f\n", -val);
            }
        }
    }
    
    
    free_state(qs);
}

void quantum_parallelism_boolean_or() {
    printf("\n========================================\n");
    printf("2. КВАНТОВЫЙ ПАРАЛЛЕЛИЗМ ДЛЯ БУЛЕВОЙ ФУНКЦИИ OR\n");
    printf("========================================\n\n");
    
    QuantumState *qs = init_state(3);
    
    int truth_table_or[4] = {0, 1, 1, 1};
    
    printf("Шаг 0: Инициализация\n");
    print_state(qs, "Начальное состояние |000>");
    
    printf("Шаг 1: Применяем преобразование Адамара к x1 и x2\n");
    apply_hadamard(qs, 2);
    apply_hadamard(qs, 1);
    print_state(qs, "После H(x1) и H(x2)");
    
    printf("Шаг 2: Применяем для функции OR\n");
    apply_boolean_oracle(qs, truth_table_or);
    print_state(qs, "После применения U_f (OR)");
    
    printf("Анализ результата:\n");
    for (int i = 0; i < 8; i++) {
        double val = get_state_value(qs, i);
        if (fabs(val) > 1e-6) {
            int x1 = (i >> 2) & 1;
            int x2 = (i >> 1) & 1;
            int y = i & 1;
            printf("  f(%d%d) = %d\n", x1, x2, y);
        }
    }
    
    free_state(qs);
}

void quantum_parallelism_arithmetic() {
    printf("\n========================================\n");
    printf("3. КВАНТОВЫЙ ПАРАЛЛЕЛИЗМ ДЛЯ АРИФМЕТИЧЕСКОЙ ФУНКЦИИ\n");
    printf("   f(x) = 2^x mod 3  (более подходящая функция)\n");
    printf("========================================\n\n");
    
    QuantumState *qs = init_state(4); 
    
    int a = 2;  
    int m = 3;  
    
    printf("Шаг 0: Инициализация\n");
    print_state(qs, "Начальное состояние |0000>");
    
    printf("Шаг 1: Применяем преобразование Адамара к кубитам x\n");
    apply_hadamard(qs, 3);  
    apply_hadamard(qs, 2);  
    print_state(qs, "После H(x1) и H(x2)");
    
    printf("Шаг 2: Применяем для f(x) = %d^x mod %d\n", a, m);
    apply_arithmetic_oracle(qs, a, m);
    print_state(qs, "После применения U_f");
    
    printf("Таблица значений f(x) = %d^x mod %d:\n", a, m);
    printf("  x  |  f(x)\n");
    printf("  ----------\n");
    
    for (int x = 0; x < 4; x++) {
        int fx = 1;
        for (int j = 0; j < x; j++) {
            fx = (fx * a) % m;
        }
        fx = fx % m;
        printf("  %d  |  %d (двоичное: %d%d)\n", x, fx, (fx>>1)&1, fx&1);
    }
}

int main() {
    printf("ПРАКТИЧЕСКОЕ ЗАНЯТИЕ №4: КВАНТОВЫЙ ПАРАЛЛЕЛИЗМ\n");
    printf("Задание 1: Моделирование эффекта квантового параллелизма\n");
    printf("========================================================\n\n");
    
    quantum_parallelism_boolean();
    
    quantum_parallelism_boolean_or();
    
    quantum_parallelism_arithmetic();
    
    printf("\n========================================\n");
    
    return 0;
}