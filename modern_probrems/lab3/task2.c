#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Функция для возведения в степень по модулю
int mod_pow(int a, int x, int m) {
    if (m == 1) return 0;
    int result = 1;
    a = a % m;
    while (x > 0) {
        if (x & 1) {
            result = (result * a) % m;
        }
        a = (a * a) % m;
        x >>= 1;
    }
    return result;
}

// Функция для определения количества бит, необходимых для представления числа
int bit_length(int n) {
    if (n == 0) return 1;
    int length = 0;
    while (n > 0) {
        length++;
        n >>= 1;
    }
    return length;
}

// Функция для создания матрицы преобразования для f(x) = a^x mod m
int** create_mod_matrix(int a, int m, int k, int r) {
    int n = 1 << k; // число возможных значений x (2^k)
    int total_states = n * (1 << r); // x (k бит) + результат (r бит)
    
    // Выделяем память под матрицу total_states x total_states
    int** F = (int**)malloc(total_states * sizeof(int*));
    for (int i = 0; i < total_states; i++) {
        F[i] = (int*)calloc(total_states, sizeof(int));
    }
    
    // Заполняем матрицу
    for (int x = 0; x < n; x++) {
        int fx = mod_pow(a, x, m);
        
        // Для каждого возможного начального состояния регистра результата
        for (int y = 0; y < (1 << r); y++) {
            // Индекс входного состояния
            int i = (x << r) | y;
            
            // Новое значение y: y ⊕ f(x)
            int new_y = y ^ fx;
            
            // Индекс выходного состояния
            int j = (x << r) | new_y;
            
            F[i][j] = 1;
        }
    }
    
    return F;
}

int main() {
    int a, m, x_input;
    
    printf("Введите a: ");
    scanf("%d", &a);
    printf("Введите m: ");
    scanf("%d", &m);
    
    // Определяем k (число бит для x) - чтобы покрыть значения 0..m-1
    int k = bit_length(m - 1);
    // Определяем r (число бит для результата)
    int r = bit_length(m - 1);
    
    printf("k (число бит для x) = %d\n", k);
    printf("r (число бит для результата) = %d\n", r);
    printf("Допустимые значения x: 0..%d\n", (1 << k) - 1);
    
    // Запрашиваем x у пользователя с проверкой
    int max_x = (1 << k) - 1;
    do {
        printf("Введите x для тестирования (0..%d): ", max_x);
        scanf("%d", &x_input);
        if (x_input < 0 || x_input > max_x) {
            printf("Ошибка: x должно быть в диапазоне [0, %d]\n", max_x);
        }
    } while (x_input < 0 || x_input > max_x);
    
    // Создаем матрицу преобразования
    int** F = create_mod_matrix(a, m, k, r);
    
    int n = 1 << k;
    int total_states = n * (1 << r);
    
    // Выводим матрицу F (первые строки для наглядности)
    printf("\nМатрица F (первые %dx%d):\n", 
           total_states < 8 ? total_states : 8,
           total_states < 8 ? total_states : 8);
    
    int size_to_print = total_states < 8 ? total_states : 8;
    for (int i = 0; i < size_to_print; i++) {
        for (int j = 0; j < size_to_print; j++) {
            printf("%d ", F[i][j]);
        }
        printf("\n");
    }
    
    // Вычисляем f(x)
    int fx = mod_pow(a, x_input, m);
    printf("\nf(%d) = %d^%d mod %d = %d\n", x_input, a, x_input, m, fx);
    
    if (r <= 8) {
        printf("Двоичное представление %d: ", fx);
        for (int bit = r-1; bit >= 0; bit--) {
            printf("%d", (fx >> bit) & 1);
        }
        printf(" (%d бит)\n", r);
    }
    
    // Пример: входное состояние с y=0 (все биты результата = 0)
    int y_input = 0;
    int input_index = (x_input << r) | y_input;
    
    // Создаем входной вектор состояния
    int* input_state = (int*)calloc(total_states, sizeof(int));
    input_state[input_index] = 1;
    
    // Вычисляем выходное состояние
    int* output_state = (int*)calloc(total_states, sizeof(int));
    for (int i = 0; i < total_states; i++) {
        for (int j = 0; j < total_states; j++) {
            output_state[i] += F[i][j] * input_state[j];
        }
    }
    
    // Находим выходное состояние (единственное с 1)
    int output_index = -1;
    for (int i = 0; i < total_states; i++) {
        if (output_state[i] == 1) {
            output_index = i;
            break;
        }
    }
    
    if (output_index == -1) {
        printf("Ошибка: не найдено выходное состояние!\n");
    } else {
        // Декодируем выходное состояние
        int x_out = output_index >> r;
        int new_y = output_index & ((1 << r) - 1);
        
        printf("\nВходное состояние:\n");
        printf("  x = %d (двоичное: ", x_input);
        for (int bit = k-1; bit >= 0; bit--) {
            printf("%d", (x_input >> bit) & 1);
        }
        printf(")\n");
        
        printf("  y = %d (двоичное: ", y_input);
        for (int bit = r-1; bit >= 0; bit--) {
            printf("%d", (y_input >> bit) & 1);
        }
        printf(")\n");
        
        printf("\nВыходное состояние:\n");
        printf("  x = %d (двоичное: ", x_out);
        for (int bit = k-1; bit >= 0; bit--) {
            printf("%d", (x_out >> bit) & 1);
        }
        printf(")\n");
        
        printf("  y = %d (двоичное: ", new_y);
        for (int bit = r-1; bit >= 0; bit--) {
            printf("%d", (new_y >> bit) & 1);
        }
        printf(")\n");
        
        printf("\nПроверка: y ⊕ f(x) = ");
        printf("%d ⊕ %d = %d\n", y_input, fx, new_y);
        
        if (new_y == (y_input ^ fx)) {
            printf("✓ Вычисление корректно!\n");
        } else {
            printf("✗ Ошибка в вычислении!\n");
        }
    }
    
    // Освобождаем память
    for (int i = 0; i < total_states; i++) {
        free(F[i]);
    }
    free(F);
    free(input_state);
    free(output_state);
    
    return 0;
}