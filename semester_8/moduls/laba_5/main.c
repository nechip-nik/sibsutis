#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N_STATES 10
#define N_STEPS 100

double exponential_random(double lambda) {
    double u = rand() / (RAND_MAX + 1.0);
    return -log(1 - u) / lambda;
}

void shuffle_indices(int *indices, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
}

void generate_markov_chain_unique(double P[N_STATES][N_STATES], int n_steps, 
                                int initial_state, int *states, double *values) {
    // Инициализация индексов
    int indices[n_steps];
    for (int i = 0; i < n_steps; i++) {
        indices[i] = i;
    }
    shuffle_indices(indices, n_steps);

    // Инициализация состояний и значений
    states[0] = initial_state;
    for (int i = 0; i < n_steps; i++) {
        values[i] = 0.0;
        states[i+1] = 0;
    }

    for (int i = 0; i < n_steps; i++) {
        int current_idx = indices[i];
        
        // Генерация значения
        values[current_idx] = exponential_random(1.0);
        
        // Выбор следующего состояния
        double r = (double)rand() / RAND_MAX;
        double sum = 0.0;
        int current_state = states[current_idx];
        
        for (int j = 0; j < N_STATES; j++) {
            sum += P[current_state][j];
            if (r <= sum) {
                if (current_idx + 1 < n_steps + 1) {  // Проверка границ
                    states[current_idx + 1] = j;
                }
                break;
            }
        }
    }
    
    // Нормировка значений
    double min_val = values[0], max_val = values[0];
    for (int i = 1; i < n_steps; i++) {
        if (values[i] < min_val) min_val = values[i];
        if (values[i] > max_val) max_val = values[i];
    }
    
    for (int i = 0; i < n_steps; i++) {
        values[i] = (values[i] - min_val) / (max_val - min_val);
    }
}

// Оригинальная генерация цепи Маркова (для сравнения)
void generate_markov_chain(double P[N_STATES][N_STATES], int n_steps, 
                         int initial_state, int *states, double *values) {
    states[0] = initial_state;
    
    for (int i = 0; i < n_steps; i++) {
        values[i] = exponential_random(1.0);
        
        double r = (double)rand() / RAND_MAX;
        double sum = 0.0;
        int current_state = states[i];
        
        for (int j = 0; j < N_STATES; j++) {
            sum += P[current_state][j];
            if (r <= sum) {
                states[i+1] = j;
                break;
            }
        }
    }
    
    double min_val = values[0], max_val = values[0];
    for (int i = 1; i < n_steps; i++) {
        if (values[i] < min_val) min_val = values[i];
        if (values[i] > max_val) max_val = values[i];
    }
    
    for (int i = 0; i < n_steps; i++) {
        values[i] = (values[i] - min_val) / (max_val - min_val);
    }
}

// Вычисление автокорреляции
void autocorrelation(double *x, int n, int max_lag, double *acf) {
    double mean = 0.0, var = 0.0;
    
    for (int i = 0; i < n; i++) {
        mean += x[i];
    }
    mean /= n;
    
    for (int i = 0; i < n; i++) {
        var += (x[i] - mean) * (x[i] - mean);
    }
    var /= n;
    
    for (int lag = 0; lag <= max_lag; lag++) {
        double cov = 0.0;
        for (int i = 0; i < n - lag; i++) {
            cov += (x[i] - mean) * (x[i + lag] - mean);
        }
        acf[lag] = cov / (n * var);
    }
}

void print_frequencies(int freq[N_STATES], const char *chain_name, int total_steps) {
    printf("\nКоличество попаданий для %s:\n", chain_name);
    printf("+------------+------------+-------------+\n");
    printf("| Состояние  | Количество | Процент (%%) |\n");
    printf("+------------+------------+-------------+\n");
    
    for (int i = 0; i < N_STATES; i++) {
        printf("|    %2d      |    %4d    |    %6.2f    |\n", 
               i, freq[i], 100.0 * freq[i] / total_steps);
    }
    printf("+------------+------------+-------------+\n");
    printf("Всего шагов: %d\n", total_steps);
}

int main() {
    srand(time(NULL));
    
    // Матрицы переходов
    double P1[N_STATES][N_STATES] = {
        {0.7, 0.06, 0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 0.03},
        {0.03, 0.7, 0.06, 0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 0.03},
        {0.03, 0.03, 0.7, 0.06, 0.03, 0.03, 0.03, 0.03, 0.03, 0.03},
        {0.03, 0.03, 0.03, 0.7, 0.06, 0.03, 0.03, 0.03, 0.03, 0.03},
        {0.03, 0.03, 0.03, 0.03, 0.7, 0.06, 0.03, 0.03, 0.03, 0.03},
        {0.03, 0.03, 0.03, 0.03, 0.03, 0.7, 0.06, 0.03, 0.03, 0.03},
        {0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 0.7, 0.06, 0.03, 0.03},
        {0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 0.7, 0.06, 0.03},
        {0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 0.7, 0.06},
        {0.06, 0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 0.7}
    };
    
    double P2[N_STATES][N_STATES];
    for (int i = 0; i < N_STATES; i++) {
        for (int j = 0; j < N_STATES; j++) {
            P2[i][j] = 0.1;
        }
    }
    
    // Важно: правильное выделение памяти
    int states1[N_STEPS + 1];  // +1 для начального состояния
    double values1[N_STEPS];
    int states2[N_STEPS + 1];
    double values2[N_STEPS];
    
    // Инициализация массивов нулями
    for (int i = 0; i < N_STEPS + 1; i++) {
        states1[i] = 0;
        states2[i] = 0;
    }
    for (int i = 0; i < N_STEPS; i++) {
        values1[i] = 0.0;
        values2[i] = 0.0;
    }
    
    printf("Генерация цепи 1 с уникальными индексами...\n");
    generate_markov_chain_unique(P1, N_STEPS, 0, states1, values1);
    
    printf("Генерация цепи 2 с уникальными индексами...\n");
    generate_markov_chain_unique(P2, N_STEPS, 0, states2, values2);
    // Сохранение результатов
    FILE *f1 = fopen("chain1_unique.txt", "w");
    FILE *f2 = fopen("chain2_unique.txt", "w");
    
    fprintf(f1, "Step\tState\tValue\n");
    fprintf(f2, "Step\tState\tValue\n");
    
    for (int i = 0; i < N_STEPS; i++) {
        fprintf(f1, "%d\t%d\t%.4f\n", i, states1[i], values1[i]);
        fprintf(f2, "%d\t%d\t%.4f\n", i, states2[i], values2[i]);
    }
    
    fclose(f1);
    fclose(f2);
    
    // Подсчет частот состояний
    int freq1[N_STATES] = {0}, freq2[N_STATES] = {0};
    
    for (int i = 0; i <= N_STEPS; i++) {
        freq1[states1[i]]++;
        freq2[states2[i]]++;
    }
    
    // Вывод статистики
    print_frequencies(freq1, "цепи 1 (P1)", N_STEPS + 1);
    print_frequencies(freq2, "цепи 2 (P2)", N_STEPS + 1);
    
    // Анализ автокорреляции
    int max_lag = 20;
    double acf1[max_lag + 1], acf2[max_lag + 1];
    
    autocorrelation(values1, N_STEPS, max_lag, acf1);
    autocorrelation(values2, N_STEPS, max_lag, acf2);
    
    printf("\nАвтокорреляция для цепи 1:\n");
    for (int lag = 0; lag <= max_lag; lag++) {
        printf("Лаг %d: %.4f\n", lag, acf1[lag]);
    }
    
    printf("\nАвтокорреляция для цепи 2:\n");
    for (int lag = 0; lag <= max_lag; lag++) {
        printf("Лаг %d: %.4f\n", lag, acf2[lag]);
    }
    
    printf("\nРезультаты сохранены в файлы chain1_unique.txt и chain2_unique.txt\n");
    return 0;
}