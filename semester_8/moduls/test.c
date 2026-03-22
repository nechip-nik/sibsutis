#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Константы
#define A 0.551  // Значение a
#define B -0.635 // Значение b
#define F1 0.2382 // Значение F(1) - граница между интервалами
#define N 100000  // Количество случайных точек

// Функция для генерации случайного числа в диапазоне [0,1]
double uniform_random() {
    return rand() / (double)RAND_MAX;
}

// Обратная функция для первого интервала (-0.2, 1)
double inverse_function_1(double u) {
    return pow((4 * u) / A, 0.25) - 0.2;
}

// Обратная функция для второго интервала (1, 2.5)
double inverse_function_2(double u) {
    return 2.5 - sqrt((0.2382 - u) / (0.3175));
}

int main() {
    // Инициализация генератора случайных чисел
    srand(time(NULL));

    // Открываем файл для записи точек
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Ошибка при открытии файла!\n");
        return 1;
    }

    // Генерация случайных точек
    for (int i = 0; i < N; i++) {
        double u = uniform_random();
        double x;

        // Выбираем нужную обратную функцию в зависимости от u
        if (u < F1) {
            x = inverse_function_1(u);
        } else {
            x = inverse_function_2(u);
        }

        // Записываем точку в файл
        fprintf(file, "%f\n", x);
    }

    // Закрываем файл
    fclose(file);
    printf("✅ Генерация завершена! Данные записаны в output.txt\n");

    return 0;
}
