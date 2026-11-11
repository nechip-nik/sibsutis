#include <stdio.h>
#include <math.h>

// ============================================================
// ТРИ ФУНКЦИИ
// ============================================================

// Функция 1: f(x, y) = x^2 - 6x + y^2 - 8y
double func1(double x, double y) {
    return x * x - 6 * x + y * y - 8 * y;
}

// Функция 2: f(x, y) = x^2 - 6x + 2y^2 - 8y
double func2(double x, double y) {
    return x * x - 6 * x + 2 * y * y - 8 * y;
}

// Функция 3: f(x, y) = (x - 2)^2 + (x - y^2)^2
double func3(double x, double y) {
    double term1 = (x - 2) * (x - 2);
    double term2 = (x - y * y) * (x - y * y);
    return term1 + term2;
}

// ============================================================
// ТЕРНАРНЫЙ ПОИСК ПО ОДНОЙ ПЕРЕМЕННОЙ (ВНУТРЕННИЙ)
// ============================================================

double ternary_1d(double (*f)(double, double), double a, double b,
                  double fixed_var, int fixed_dim, double tol) {
    int iter = 0;
    int max_iter = 1000;
    
    while (b - a > tol && iter < max_iter) {
        iter++;
        
        double m1 = a + (b - a) / 3.0;
        double m2 = b - (b - a) / 3.0;
        
        double f1, f2;
        if (fixed_dim == 0) {
            f1 = f(fixed_var, m1);
            f2 = f(fixed_var, m2);
        } else {
            f1 = f(m1, fixed_var);
            f2 = f(m2, fixed_var);
        }
        
        if (f1 < f2) {
            b = m2;
        } else {
            a = m1;
        }
    }
    
    return (a + b) / 2.0;
}

// ============================================================
// ВЛОЖЕННЫЙ ТЕРНАРНЫЙ ПОИСК (ВНЕШНИЙ)
// ============================================================

void nested_ternary_search(double (*f)(double, double),
                           double x_min, double x_max,
                           double y_min, double y_max,
                           double tol, int max_iter,
                           double *x_opt, double *y_opt, double *f_opt,
                           int *iter_count) {
    
    double x_left = x_min, x_right = x_max;
    double y_left = y_min, y_right = y_max;
    
    int iter = 0;
    
    printf("\n");
    printf("============================================================\n");
    printf("ВЛОЖЕННЫЙ ТЕРНАРНЫЙ ПОИСК\n");
    printf("============================================================\n");
    printf("\n");
    printf("Интервал по x: [%.4f, %.4f]\n", x_min, x_max);
    printf("Интервал по y: [%.4f, %.4f]\n", y_min, y_max);
    printf("Точность: %.0e\n", tol);
    printf("\n");
    printf("------------------------------------------------------------\n");
    printf("Итерация |     x1     |     y1*    |   f(x1,y1)  |     x2     |     y2*    |   f(x2,y2)  |  Действие\n");
    printf("------------------------------------------------------------\n");
    
    while ((x_right - x_left) > tol && iter < max_iter) {
        iter++;
        
        // Делим интервал по x на 3 части
        double x1 = x_left + (x_right - x_left) / 3.0;
        double x2 = x_right - (x_right - x_left) / 3.0;
        
        // Для каждого x находим оптимальный y (вложенный поиск)
        double y1 = ternary_1d(f, y_left, y_right, x1, 0, tol / 10.0);
        double y2 = ternary_1d(f, y_left, y_right, x2, 0, tol / 10.0);
        
        double f1 = f(x1, y1);
        double f2 = f(x2, y2);
        
        printf("  %2d     | %10.6f | %10.6f | %11.6f | %10.6f | %10.6f | %11.6f | ",
               iter, x1, y1, f1, x2, y2, f2);
        
        if (f1 < f2) {
            x_right = x2;
            printf("отбрасываем ПРАВУЮ часть [%.4f, %.4f]\n", x2, x_right);
        } else {
            x_left = x1;
            printf("отбрасываем ЛЕВУЮ часть [%.4f, %.4f]\n", x_left, x1);
        }
    }
    
    printf("------------------------------------------------------------\n");
    printf("Внешних итераций: %d\n", iter);
    printf("Длина интервала по x: %.2e\n", x_right - x_left);
    printf("\n");
    
    // Финальное уточнение
    double x_center = (x_left + x_right) / 2.0;
    double y_final = ternary_1d(f, y_min, y_max, x_center, 0, tol / 100.0);
    double x_final = ternary_1d(f, x_left, x_right, y_final, 1, tol / 100.0);
    
    // Ещё одно уточнение
    y_final = ternary_1d(f, y_min, y_max, x_final, 0, tol / 1000.0);
    x_final = ternary_1d(f, x_left, x_right, y_final, 1, tol / 1000.0);
    
    *x_opt = x_final;
    *y_opt = y_final;
    *f_opt = f(*x_opt, *y_opt);
    *iter_count = iter;
}

// ============================================================
// ОСНОВНАЯ ПРОГРАММА
// ============================================================

int main() {
    double x_min = -10.0, x_max = 10.0;
    double y_min = -10.0, y_max = 10.0;
    double tol = 1e-9;
    int max_iter = 100;
    
    double x_opt, y_opt, f_opt;
    int iter_count;
    
    printf("\n");
    printf("============================================================\n");
    printf("ВЫБОР ФУНКЦИИ\n");
    printf("============================================================\n");
    printf("\n");
    printf("1) f(x, y) = x^2 - 6x + y^2 - 8y\n");
    printf("2) f(x, y) = x^2 - 6x + 2y^2 - 8y\n");
    printf("3) f(x, y) = (x - 2)^2 + (x - y^2)^2\n");
    printf("\n");
    printf("Введите номер функции (1, 2 или 3): ");
    
    int choice;
    scanf("%d", &choice);
    getchar();
    
    double (*func)(double, double);
    char *func_name;
    
    switch (choice) {
        case 1:
            func = func1;
            func_name = "f(x, y) = x^2 - 6x + y^2 - 8y";
            break;
        case 2:
            func = func2;
            func_name = "f(x, y) = x^2 - 6x + 2y^2 - 8y";
            break;
        case 3:
            func = func3;
            func_name = "f(x, y) = (x - 2)^2 + (x - y^2)^2";
            break;
        default:
            printf("Неверный выбор!\n");
            return 1;
    }
    
    printf("\n");
    printf("============================================================\n");
    printf("ФУНКЦИЯ: %s\n", func_name);
    printf("============================================================\n");
    printf("\n");
    printf("Интервал: x ∈ [-10, 10], y ∈ [-10, 10]\n");
    printf("\n");
    printf("Нажмите Enter для начала поиска...");
    getchar();
    
    nested_ternary_search(func, x_min, x_max, y_min, y_max,
                          tol, max_iter, &x_opt, &y_opt, &f_opt, &iter_count);
    
    printf("\n");
    printf("============================================================\n");
    printf("РЕЗУЛЬТАТ\n");
    printf("============================================================\n");
    printf("\n");
    printf("  x* = %.10f\n", x_opt);
    printf("  y* = %.10f\n", y_opt);
    printf("  f(x*, y*) = %.10f\n", f_opt);
    printf("\n");
    printf("  Внешних итераций: %d\n", iter_count);
    printf("\n");
    printf("============================================================\n");
    
    return 0;
}