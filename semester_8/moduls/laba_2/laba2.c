#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Функция для генерации случайных точек внутри области
void generate_points(int n, double *x, double *y) {
    for (int i = 0; i < n; i++) {
        // Генерация x с плотностью, пропорциональной sin(x)
        double u = (double)rand() / RAND_MAX; // Случайное u в [0, 1]
        x[i] = acos(1 - 2 * u);              // Обратное преобразование для sin(x)

        // Генерация y равномерно в пределах [2 - 2*sin(x), 2 - sin(x)]
        double v = (double)rand() / RAND_MAX; // Случайное v в [0, 1]
        y[i] = 2 - 2 * sin(x[i]) + v * sin(x[i]);
    }
}

// Функция для генерации точек на границах области с равномерным распределением
void generate_boundary_points(int n, double *x, double *y_lower, double *y_upper) {
    for (int i = 0; i < n; i++) {
        // Генерация x равномерно в [0, pi]
        x[i] = (double)rand() / RAND_MAX * M_PI; // Случайное x в [0, pi]

        // Точки на границах
        y_lower[i] = 2 - 2 * sin(x[i]); // Нижняя граница
        y_upper[i] = 2 - sin(x[i]);     // Верхняя граница
    }
}

int main() {
    srand(time(NULL)); // Инициализация генератора случайных чисел

    int n = 1000; // Количество точек
    double x[n], y[n];
    double x_boundary[n], y_lower[n], y_upper[n];

    // Генерация точек внутри области
    generate_points(n, x, y);

    // Генерация точек на границах области
    generate_boundary_points(n, x_boundary, y_lower, y_upper);

    // Вывод точек внутри области в файл
    FILE *file_inner = fopen("inner_points.txt", "w");
    if (file_inner == NULL) {
        printf("Ошибка открытия файла inner_points.txt\n");
        return 1;
    }
    for (int i = 0; i < n; i++) {
        fprintf(file_inner, "%f %f\n", x[i], y[i]);
    }
    fclose(file_inner);

    // Вывод точек на границах области в файл
    FILE *file_boundary = fopen("boundary_points.txt", "w");
    if (file_boundary == NULL) {
        printf("Ошибка открытия файла boundary_points.txt\n");
        return 1;
    }
    for (int i = 0; i < n; i++) {
        fprintf(file_boundary, "%f %f %f\n", x_boundary[i], y_lower[i], y_upper[i]);
    }
    fclose(file_boundary);

    printf("Точки сохранены в файлы inner_points.txt и boundary_points.txt\n");

    return 0;
}