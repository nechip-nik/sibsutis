#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define PI 3.14159265358979323846

// Функция для вычисления значения y = sin(x)
double f_sin(double x) {
    return sin(x);
}

// Функция для вычисления значения y = 2 - x
double f_linear(double x) {
    return 2.0 - x;
}

// Функция для нахождения пересечения двух функций
double find_intersection(double (*f1)(double), double (*f2)(double)) {
    double a = 0.0, b = 2.0;
    double c;
    do {
        c = (a + b) / 2.0;
        if ((*f1)(c) > (*f2)(c))
            b = c;
        else
            a = c;
    } while (b - a > 1e-6);
    return c;
}

// Функция для расчета общей площади
double calculate_area(double x0) {
    double area_rect = 2.0 * x0;     // Площадь прямоугольника
    double area_curve = 2.0 - x0 - cos(x0);  // Площадь криволинейной части
    return area_rect + area_curve;
}

// Функция для генерации случайных точек
void generate_random_points(int n_points, double x0, double total_area) {
    srand(time(NULL)); // Инициализация генератора случайных чисел

    for (int i = 0; i < n_points; ++i) {
        double xi = (double)rand() / RAND_MAX; // Случайное число от 0 до 1

        double x;
        if (xi <= x0 / total_area) { // Прямоугольная часть
            x = xi * x0 / (x0 / total_area);
        } else { // Криволинейная часть
            double temp = sqrt((xi - x0 / total_area) * (2.0 - x0 - cos(x0)));
            x = acos(1.0 - temp);
        }

        // Ограничиваем диапазон генерации до 2.0
        // if (x > 2.0) {
        //     continue; // Пропускаем точки, выходящие за пределы 2.0
        // }

        printf("%lf %lf\n", x, f_sin(x));
    }
}

int main() {
    double x0 = find_intersection(f_sin, f_linear); // Найти точку пересечения
    double total_area = calculate_area(x0); // Рассчитать общую площадь

    int n_points = 10000; // Количество генерируемых точек
    generate_random_points(n_points, x0, total_area); // Генерируем случайные точки

    return 0;
}