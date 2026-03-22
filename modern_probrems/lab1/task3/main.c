#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define MAX_QUBITS 8

typedef struct {
    double* amplitudes;  // амплитуды вероятностей для всех состояний
    int num_qubits;      
} QuantumRegister;


QuantumRegister* create_quantum_register(int num_qubits) {
    QuantumRegister* reg = (QuantumRegister*)malloc(sizeof(QuantumRegister));
    reg->num_qubits = num_qubits;
    int num_states = 1 << num_qubits; // 2^num_qubits
    reg->amplitudes = (double*)malloc(num_states * sizeof(double));
    
    for (int i = 0; i < num_states; i++) {
        reg->amplitudes[i] = 0.0;
    }
    reg->amplitudes[0] = 1.0;
    
    return reg;
}

void free_quantum_register(QuantumRegister* reg) {
    free(reg->amplitudes);
    free(reg);
}


void normalize(QuantumRegister* reg) {
    int num_states = 1 << reg->num_qubits;
    double norm = 0.0;
    
    for (int i = 0; i < num_states; i++) {
        norm += reg->amplitudes[i] * reg->amplitudes[i];
    }
    
    norm = sqrt(norm);
    for (int i = 0; i < num_states; i++) {
        reg->amplitudes[i] /= norm;
    }
}

// Функция для установки состояния регистра
void set_register_state(QuantumRegister* reg, double* new_amplitudes) {
    int num_states = 1 << reg->num_qubits;
    memcpy(reg->amplitudes, new_amplitudes, num_states * sizeof(double));
    normalize(reg);
}

// Функция для измерения всего регистра
int measure_full_register(QuantumRegister* reg) {
    int num_states = 1 << reg->num_qubits;
    
    double* probabilities = (double*)malloc(num_states * sizeof(double));
    double cumulative_prob = 0.0;
    
    for (int i = 0; i < num_states; i++) {
        probabilities[i] = reg->amplitudes[i] * reg->amplitudes[i];
        cumulative_prob += probabilities[i];
    }
    
    // Генерируем случайное число для определения результата измерения
    double random_val = ((double)rand() / RAND_MAX) * cumulative_prob;
    
    int result_state = 0;
    double current_prob = 0.0;
    
    for (int i = 0; i < num_states; i++) {
        current_prob += probabilities[i];
        if (random_val <= current_prob) {
            result_state = i;
            break;
        }
    }
    
    for (int i = 0; i < num_states; i++) {
        reg->amplitudes[i] = (i == result_state) ? 1.0 : 0.0;
    }
    
    free(probabilities);
    return result_state;
}

// Функция для измерения отдельных кубитов
int measure_single_qubit(QuantumRegister* reg, int qubit_index) {
    int num_states = 1 << reg->num_qubits;
    
    // Вычисляем вероятность получить 0 и 1 для данного кубита
    double prob0 = 0.0;
    double prob1 = 0.0;
    
    for (int i = 0; i < num_states; i++) {
        // Проверяем значение измеряемого кубита в состоянии i
        if ((i >> qubit_index) & 1) {
            prob1 += reg->amplitudes[i] * reg->amplitudes[i];
        } else {
            prob0 += reg->amplitudes[i] * reg->amplitudes[i];
        }
    }
    
    // Генерируем случайное число для определения результата измерения
    double random_val = (double)rand() / RAND_MAX;
    int result;
    
    if (random_val < prob0) {
        result = 0;

        for (int i = 0; i < num_states; i++) {
            if ((i >> qubit_index) & 1) {
                reg->amplitudes[i] = 0.0;
            }
        }
    } else {
        result = 1;
        for (int i = 0; i < num_states; i++) {
            if (!((i >> qubit_index) & 1)) {
                reg->amplitudes[i] = 0.0;
            }
        }
    }
    
    normalize(reg);
    return result;
}


void int_to_binary(int n, int num_bits, char* buffer) {
    for (int i = num_bits - 1; i >= 0; i--) {
        buffer[num_bits - 1 - i] = ((n >> i) & 1) ? '1' : '0';
    }
    buffer[num_bits] = '\0';
}


void print_register_state(QuantumRegister* reg) {
    int num_states = 1 << reg->num_qubits;
    char binary[ MAX_QUBITS + 1 ];
    
    printf("Состояние регистра:\n");
    for (int i = 0; i < num_states; i++) {
        if (fabs(reg->amplitudes[i]) > 1e-10) {
            int_to_binary(i, reg->num_qubits, binary);
            printf("|%s⟩: %.3f (вероятность: %.3f)\n", 
                   binary, reg->amplitudes[i], reg->amplitudes[i] * reg->amplitudes[i]);
        }
    }
}

void print_probabilities(QuantumRegister* reg) {
    int num_states = 1 << reg->num_qubits;
    char binary[ MAX_QUBITS + 1 ];
    
    printf("Вероятности состояний:\n");
    for (int i = 0; i < num_states; i++) {
        double prob = reg->amplitudes[i] * reg->amplitudes[i];
        if (prob > 1e-10) {
            int_to_binary(i, reg->num_qubits, binary);
            printf("P(|%s⟩) = %.3f\n", binary, prob);
        }
    }
}

int main() {
    srand(time(NULL));
    
    printf("=== МОДЕЛИРОВАНИЕ ИЗМЕРЕНИЯ КВАНТОВОГО РЕГИСТРА ===\n\n");
    
    // Пример 1: Регистр из 2 кубитов в состоянии Белла
    printf("=== Пример 1: Состояние Белла (2 кубита) ===\n");
    QuantumRegister* bell_state = create_quantum_register(2);
    double bell_amplitudes[] = {1.0/sqrt(2.0), 0.0, 0.0, 1.0/sqrt(2.0)};
    set_register_state(bell_state, bell_amplitudes);
    
    print_register_state(bell_state);
    print_probabilities(bell_state);
    
    // Измерение всего регистра
    int result = measure_full_register(bell_state);
    char binary_result[3];
    int_to_binary(result, 2, binary_result);
    printf("\nРезультат измерения всего регистра: |%s⟩\n", binary_result);
    printf("Состояние после измерения:\n");
    print_register_state(bell_state);
    
    free_quantum_register(bell_state);
    
    // Пример 2: Регистр из 3 кубитов (пример из условия)
    printf("\n=== Пример 2: Регистр из 3 кубитов ===\n");
    QuantumRegister* three_qubit = create_quantum_register(3);
    double three_amplitudes[] = {0.5, 0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5};
    set_register_state(three_qubit, three_amplitudes);
    
    print_register_state(three_qubit);
    print_probabilities(three_qubit);
    
    // Измерение только третьего (младшего) кубита
    printf("\nИзмерение третьего кубита (индекс 0):\n");
    int single_result = measure_single_qubit(three_qubit, 0);
    printf("Результат измерения: %d\n", single_result);
    printf("Состояние после измерения одного кубита:\n");
    print_register_state(three_qubit);
    
    free_quantum_register(three_qubit);
    
    // Пример 3: Статистика измерений
    printf("\n=== Пример 3: Статистика 1000 измерений ===\n");
    QuantumRegister* test_reg = create_quantum_register(2);
    double test_amplitudes[] = {0.5, 0.5, 0.5, 0.5};
    set_register_state(test_reg, test_amplitudes);
    
    int stats[4] = {0}; // счетчики для |00⟩, |01⟩, |10⟩, |11⟩
    
    for (int i = 0; i < 1000; i++) {
        QuantumRegister* temp_reg = create_quantum_register(2);
        set_register_state(temp_reg, test_amplitudes);
        int res = measure_full_register(temp_reg);
        stats[res]++;
        free_quantum_register(temp_reg);
    }
    
    printf("Статистика измерений равновероятной суперпозиции:\n");
    printf("|00⟩: %d (%.1f%%)\n", stats[0], (stats[0]/1000.0)*100);
    printf("|01⟩: %d (%.1f%%)\n", stats[1], (stats[1]/1000.0)*100);
    printf("|10⟩: %d (%.1f%%)\n", stats[2], (stats[2]/1000.0)*100);
    printf("|11⟩: %d (%.1f%%)\n", stats[3], (stats[3]/1000.0)*100);
    
    free_quantum_register(test_reg);
    
    return 0;
}