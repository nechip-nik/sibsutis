#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define NUM_QUBITS 3
#define STATE_SIZE 8  // 2^3 = 8

// Структура для представления квантового состояния трех кубитов
typedef struct {
    double amplitudes[STATE_SIZE];  // амплитуды вероятностей для |000⟩, |001⟩, ..., |111⟩
} QuantumState;

// Структура для представления результата измерения
typedef struct {
    int classical_bits[NUM_QUBITS];  // классические биты (000, 001, ..., 111)
    QuantumState state;              // результирующее состояние после измерения
} MeasurementResult;

// Функция для генерации случайного квантового состояния
QuantumState generate_random_state() {
    QuantumState state;
    double sum_squares = 0.0;
    
    // Генерируем случайные амплитуды
    for (int i = 0; i < STATE_SIZE; i++) {
        state.amplitudes[i] = (double)rand() / RAND_MAX * 2.0 - 1.0;  // от -1 до 1
        sum_squares += state.amplitudes[i] * state.amplitudes[i];
    }
    
    // Нормализуем состояние
    double norm = sqrt(sum_squares);
    for (int i = 0; i < STATE_SIZE; i++) {
        state.amplitudes[i] /= norm;
    }
    
    return state;
}

// Функция для применения преобразования Адамара к одному кубиту
QuantumState apply_hadamard_single(QuantumState state, int qubit_index) {
    QuantumState result = state;
    
    int block_size = 1 << (NUM_QUBITS - qubit_index - 1);
    int step_size = block_size * 2;
    
    for (int i = 0; i < STATE_SIZE; i += step_size) {
        for (int j = 0; j < block_size; j++) {
            int idx1 = i + j;
            int idx2 = i + j + block_size;
            
            double a = result.amplitudes[idx1];
            double b = result.amplitudes[idx2];
            
            result.amplitudes[idx1] = (a + b) / sqrt(2.0);
            result.amplitudes[idx2] = (a - b) / sqrt(2.0);
        }
    }
    
    return result;
}

// Функция для применения преобразования Адамара ко всем трем кубитам
QuantumState apply_hadamard_all(QuantumState state) {
    QuantumState result = state;
    for (int i = 0; i < NUM_QUBITS; i++) {
        result = apply_hadamard_single(result, i);
    }
    return result;
}

// Функция для измерения в стандартном базисе
MeasurementResult measure_standard_basis(QuantumState state) {
    MeasurementResult result;
    
    // Вычисляем вероятности для всех состояний
    double probabilities[STATE_SIZE];
    double total_prob = 0.0;
    
    for (int i = 0; i < STATE_SIZE; i++) {
        probabilities[i] = state.amplitudes[i] * state.amplitudes[i];
        total_prob += probabilities[i];
    }
    
    // Нормализуем вероятности
    for (int i = 0; i < STATE_SIZE; i++) {
        probabilities[i] /= total_prob;
    }
    
    // Генерируем случайное число для симуляции вероятностного измерения
    double random_val = (double)rand() / RAND_MAX;
    
    // Выбираем состояние согласно вероятностям
    double cumulative_prob = 0.0;
    int measured_state = 0;
    
    for (int i = 0; i < STATE_SIZE; i++) {
        cumulative_prob += probabilities[i];
        if (random_val <= cumulative_prob) {
            measured_state = i;
            break;
        }
    }
    
    // Преобразуем номер состояния в классические биты
    for (int i = 0; i < NUM_QUBITS; i++) {
        result.classical_bits[NUM_QUBITS - 1 - i] = (measured_state >> i) & 1;
    }
    
    // Коллапсируем состояние в измеренное состояние
    for (int i = 0; i < STATE_SIZE; i++) {
        result.state.amplitudes[i] = (i == measured_state) ? 1.0 : 0.0;
    }
    
    return result;
}

// Функция для измерения в повернутом базисе
MeasurementResult measure_rotated_basis(QuantumState state) {
    MeasurementResult result;
    
    // Применяем преобразование Адамара ко всем кубитам
    QuantumState rotated_state = apply_hadamard_all(state);
    
    // Вычисляем вероятности в повернутом базисе
    double probabilities[STATE_SIZE];
    double total_prob = 0.0;
    
    for (int i = 0; i < STATE_SIZE; i++) {
        probabilities[i] = rotated_state.amplitudes[i] * rotated_state.amplitudes[i];
        total_prob += probabilities[i];
    }
    
    // Нормализуем вероятности
    for (int i = 0; i < STATE_SIZE; i++) {
        probabilities[i] /= total_prob;
    }
    
    // Генерируем случайное число для симуляции вероятностного измерения
    double random_val = (double)rand() / RAND_MAX;
    
    // Выбираем состояние согласно вероятностям
    double cumulative_prob = 0.0;
    int measured_state = 0;
    
    for (int i = 0; i < STATE_SIZE; i++) {
        cumulative_prob += probabilities[i];
        if (random_val <= cumulative_prob) {
            measured_state = i;
            break;
        }
    }
    
    // Преобразуем номер состояния в классические биты
    for (int i = 0; i < NUM_QUBITS; i++) {
        result.classical_bits[NUM_QUBITS - 1 - i] = (measured_state >> i) & 1;
    }
    
    // Коллапсируем состояние в измеренное состояние в повернутом базисе
    QuantumState collapsed_state;
    for (int i = 0; i < STATE_SIZE; i++) {
        collapsed_state.amplitudes[i] = (i == measured_state) ? 1.0 : 0.0;
    }
    
    // Применяем обратное преобразование Адамара
    result.state = apply_hadamard_all(collapsed_state);
    
    return result;
}

// Функция для получения строкового представления состояния
char* state_to_string(int state_index) {
    static char str[4];
    for (int i = 0; i < NUM_QUBITS; i++) {
        str[NUM_QUBITS - 1 - i] = ((state_index >> i) & 1) ? '1' : '0';
    }
    str[NUM_QUBITS] = '\0';
    return str;
}

// Функция для вывода состояния трех кубитов
void print_state(QuantumState state, const char* label) {
    printf("%s:\n", label);
    for (int i = 0; i < STATE_SIZE; i++) {
        if (fabs(state.amplitudes[i]) > 1e-10) {
            printf("  |%s⟩: % .3f\n", state_to_string(i), state.amplitudes[i]);
        }
    }
    
    // Выводим вероятности
    printf("Вероятности:\n");
    for (int i = 0; i < STATE_SIZE; i++) {
        double prob = state.amplitudes[i] * state.amplitudes[i];
        if (prob > 1e-10) {
            printf("  P(|%s⟩) = %.3f\n", state_to_string(i), prob);
        }
    }
    printf("\n");
}

// Функция для вывода результата измерения
void print_measurement_result(MeasurementResult result, const char* basis_name) {
    printf("Результат измерения в %s базисе:\n", basis_name);
    printf("Классические биты: ");
    for (int i = 0; i < NUM_QUBITS; i++) {
        printf("%d", result.classical_bits[i]);
    }
    
    if (strcmp(basis_name, "повернутом") == 0) {
        printf(" (соответствует |");
        for (int i = 0; i < NUM_QUBITS; i++) {
            printf("%c", result.classical_bits[i] == 0 ? '+' : '-');
        }
        printf("⟩ в повернутом базисе)");
    } else {
        printf(" (|");
        for (int i = 0; i < NUM_QUBITS; i++) {
            printf("%d", result.classical_bits[i]);
        }
        printf("⟩)");
    }
    printf("\n");
    
    printf("Результирующее состояние в стандартном базисе:\n");
    int nonzero_count = 0;
    for (int i = 0; i < STATE_SIZE; i++) {
        if (fabs(result.state.amplitudes[i]) > 1e-10) {
            printf("  |%s⟩: % .3f\n", state_to_string(i), result.state.amplitudes[i]);
            nonzero_count++;
        }
    }
    if (nonzero_count == 0) {
        printf("  (нулевое состояние)\n");
    }
    printf("\n");
}

int main() {
    // Инициализация генератора случайных чисел
    srand(time(NULL));
    
    printf("=== Моделирование измерения трех кубитов ===\n");
    
        
        // Генерируем случайное состояние
        QuantumState random_state = generate_random_state();
        print_state(random_state, "Сгенерированное начальное состояние");
        
        // Измерение в стандартном базисе
        MeasurementResult std_result = measure_standard_basis(random_state);
        print_measurement_result(std_result, "стандартном");
        
        // Измерение в повернутом базисе
        MeasurementResult rot_result = measure_rotated_basis(random_state);
        print_measurement_result(rot_result, "повернутом");
        
        printf("============================================\n\n");
    
    return 0;
}