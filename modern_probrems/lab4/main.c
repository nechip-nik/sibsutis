#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <string.h>

// ==================== ОПРЕДЕЛЕНИЯ И УТИЛИТЫ ====================

#define MAX_QUBITS 4
#define MAX_STATES (1 << MAX_QUBITS)

// Структура для квантового состояния
typedef struct {
    int n_qubits;           // количество кубитов
    int n_states;           // количество состояний = 2^n_qubits
    double complex *amps;   // амплитуды
} QuantumState;

// Инициализация состояния |0...0>
QuantumState* init_state(int n) {
    QuantumState *qs = malloc(sizeof(QuantumState));
    qs->n_qubits = n;
    qs->n_states = 1 << n;
    qs->amps = calloc(qs->n_states, sizeof(double complex));
    qs->amps[0] = 1.0 + 0.0*I;
    return qs;
}

// Освобождение памяти
void free_state(QuantumState *qs) {
    if (qs) {
        free(qs->amps);
        free(qs);
    }
}

// Вывод состояния
void print_state(QuantumState *qs, const char *label) {
    printf("%s:\n", label);
    int printed = 0;
    for (int i = 0; i < qs->n_states; i++) {
        double mag = cabs(qs->amps[i]);
        if (mag > 1e-6) {
            printf("  |");
            for (int q = qs->n_qubits - 1; q >= 0; q--) {
                printf("%d", (i >> q) & 1);
            }
            printf("⟩: %.3f + %.3fi (mag=%.3f)\n", 
                   creal(qs->amps[i]), cimag(qs->amps[i]), mag);
            printed++;
        }
    }
    if (printed == 0) printf("  (все амплитуды ~0)\n");
    printf("\n");
}

// Применение вентиля Адамара к кубиту q (0-based)
void apply_hadamard(QuantumState *qs, int q) {
    int shift = 1 << q;
    int block_size = 1 << (q + 1);
    
    for (int start = 0; start < qs->n_states; start += block_size) {
        for (int i = start; i < start + shift; i++) {
            double complex a = qs->amps[i];
            double complex b = qs->amps[i + shift];
            
            // H|a> = (|0> + (-1)^a|1>)/√2, но проще через матрицу
            qs->amps[i] = (a + b) / sqrt(2.0);
            qs->amps[i + shift] = (a - b) / sqrt(2.0);
        }
    }
}

// Применение оракула для булевой функции f(x1,x2)
void apply_oracle_boolean(QuantumState *qs, int truth_table[4]) {
    // Предполагаем, что qs имеет 3 кубита: |x1 x2 y>
    // где x1, x2 - входы, y - выход (последний кубит)
    for (int i = 0; i < qs->n_states; i++) {
        int x1 = (i >> 2) & 1;
        int x2 = (i >> 1) & 1;
        int y = i & 1;
        
        int index = (x1 << 1) | x2;  // 0..3
        int f_val = truth_table[index];
        
        // Если функция возвращает 1, меняем амплитуду для y=1
        // Реализуем U_f: |x,y> -> |x, y⊕f(x)>
        if (f_val == 1) {
            int target_state = i ^ 1;  // меняем последний кубит
            // Простая реализация: обменяем амплитуды
            double complex temp = qs->amps[i];
            qs->amps[i] = qs->amps[target_state];
            qs->amps[target_state] = temp;
        }
    }
}

// ==================== ЗАДАНИЕ 1: КВАНТОВЫЙ ПАРАЛЛЕЛИЗМ ====================

void quantum_parallelism_bool() {
    printf("\n=== ЗАДАНИЕ 1: КВАНТОВЫЙ ПАРАЛЛЕЛИЗМ (Булева функция И) ===\n");
    
    // 3 кубита: |x1 x2 y>
    QuantumState *qs = init_state(3);
    print_state(qs, "1. Начальное состояние |000>");
    
    // Применяем Адамары к первым двум кубитам (x1, x2)
    apply_hadamard(qs, 2); // x1 (кубит 2)
    apply_hadamard(qs, 1); // x2 (кубит 1)
    print_state(qs, "2. После H на x1, x2 (равномерная суперпозиция)");
    
    // Таблица истинности для AND
    int truth_table_and[4] = {0, 0, 0, 1}; // f(00)=0, f(01)=0, f(10)=0, f(11)=1
    
    // Применяем оракул
    apply_oracle_boolean(qs, truth_table_and);
    
    printf("3. После применения функции f (AND):\n");
    printf("   Состояние содержит все 4 пары (x1,x2,f(x1,x2)) одновременно:\n");
    for (int i = 0; i < 8; i++) {
        int x1 = (i >> 2) & 1;
        int x2 = (i >> 1) & 1;
        int y = i & 1;
        double mag = cabs(qs->amps[i]);
        if (mag > 1e-6) {
            printf("   |%d%d⟩|%d⟩: амплитуда %.3f\n", x1, x2, y, mag);
        }
    }
    printf("\n");
    
    free_state(qs);
}

// ==================== ЗАДАНИЕ 2: ЗАДАЧА ДОЙЧА-ЁЖИ ====================

void deutsch_jozsa_for_two_bits(int truth_table[4], const char *func_name) {
    printf("\n=== Задача Дойча-Ёжи для функции: %s ===\n", func_name);
    
    // Для функции от 2 переменных: 2 входных + 1 вспомогательный кубит
    QuantumState *qs = init_state(3); // |x1 x2 y>
    
    // Инициализация: |00>|1> (последний кубит в |1>)
    // Обнуляем все
    for (int i = 0; i < 8; i++) qs->amps[i] = 0.0;
    // Устанавливаем |001> (x1=0, x2=0, y=1)
    qs->amps[1] = 1.0 + 0.0*I; // |001> = 1 в двоичном
    
    print_state(qs, "1. Начальное состояние |001>");
    
    // Шаг 1: H на всех кубитах
    apply_hadamard(qs, 2); // x1
    apply_hadamard(qs, 1); // x2  
    apply_hadamard(qs, 0); // y
    print_state(qs, "2. После H на всех кубитах");
    
    // Шаг 2: Оракул U_f
    apply_oracle_boolean(qs, truth_table);
    print_state(qs, "3. После оракула U_f");
    
    // Шаг 3: H на входных кубитах (x1, x2)
    apply_hadamard(qs, 2); // x1
    apply_hadamard(qs, 1); // x2
    print_state(qs, "4. После H на входных кубитах");
    
    // Анализ: смотрим только на первые два кубита
    printf("5. Анализ результатов (первые два кубита):\n");
    
    double prob_zero_zero = 0.0;
    for (int i = 0; i < 8; i += 2) { // состояния где x1=0, x2=0 (первые 4 бита: 00*)
        if (((i >> 2) & 1) == 0 && ((i >> 1) & 1) == 0) {
            prob_zero_zero += cabs(qs->amps[i]) * cabs(qs->amps[i]);
            prob_zero_zero += cabs(qs->amps[i+1]) * cabs(qs->amps[i+1]);
        }
    }
    
    printf("   Вероятность получить |00> на входных кубитах: %.4f\n", prob_zero_zero);
    
    // Определяем тип функции
    int is_constant = 1;
    int first_val = truth_table[0];
    for (int i = 1; i < 4; i++) {
        if (truth_table[i] != first_val) {
            is_constant = 0;
            break;
        }
    }
    
    int is_balanced = 0;
    if (!is_constant) {
        int zeros = 0, ones = 0;
        for (int i = 0; i < 4; i++) {
            if (truth_table[i] == 0) zeros++;
            else ones++;
        }
        if (zeros == ones) is_balanced = 1;
    }
    
    printf("   Функция: ");
    if (is_constant) {
        printf("КОНСТАНТНАЯ (f(x) = %d)\n", first_val);
        printf("   Ожидаем результат |00> с вероятностью ~1.0\n");
    } else if (is_balanced) {
        printf("СБАЛАНСИРОВАННАЯ\n");
        printf("   Ожидаем результат НЕ |00> с вероятностью 1.0\n");
    } else {
        printf("НИ константная, НИ сбалансированная\n");
        printf("   Результат может быть любым\n");
    }
    
    free_state(qs);
}

// ==================== ГЛАВНАЯ ФУНКЦИЯ ====================

int main() {
    printf("========================================\n");
    printf("ПРАКТИЧЕСКОЕ ЗАНЯТИЕ №4: КВАНТОВЫЙ ПАРАЛЛЕЛИЗМ\n");
    printf("========================================\n");
    
    // ЗАДАНИЕ 1: Квантовый параллелизм
    quantum_parallelism_bool();
    
    printf("\n\n");
    printf("========================================\n");
    printf("ЗАДАНИЕ 2: ЗАДАЧА ДОЙЧА-ЁЖИ\n");
    printf("========================================\n");
    
    // Различные тестовые функции
    
    // 1. Константная функция 0
    int constant_0[4] = {0, 0, 0, 0};
    deutsch_jozsa_for_two_bits(constant_0, "Константная 0 (f=0)");
    
    // 2. Константная функция 1
    int constant_1[4] = {1, 1, 1, 1};
    deutsch_jozsa_for_two_bits(constant_1, "Константная 1 (f=1)");
    
    // 3. Сбалансированная: AND (но это не сбалансированная!)
    // AND: 0,0,0,1 - три нуля, одна единица - НЕ сбалансированная
    int and_func[4] = {0, 0, 0, 1};
    deutsch_jozsa_for_two_bits(and_func, "AND (не сбалансированная)");
    
    // 4. Сбалансированная: XOR
    int xor_func[4] = {0, 1, 1, 0}; // f(00)=0, f(01)=1, f(10)=1, f(11)=0
    deutsch_jozsa_for_two_bits(xor_func, "XOR (сбалансированная)");
    
    // 5. Другая сбалансированная
    int balanced1[4] = {1, 0, 0, 1}; // f(00)=1, f(01)=0, f(10)=0, f(11)=1
    deutsch_jozsa_for_two_bits(balanced1, "Сбалансированная 1");
    
    // 6. Ни константная, ни сбалансированная
    int neither[4] = {0, 0, 1, 1}; // f(00)=0, f(01)=0, f(10)=1, f(11)=1
    deutsch_jozsa_for_two_bits(neither, "Ни константная, ни сбалансированная");
    
    return 0;
}