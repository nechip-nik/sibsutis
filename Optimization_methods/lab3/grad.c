#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// ============================================================
// ТРИ ФУНКЦИИ И ИХ ГРАДИЕНТЫ
// ============================================================

// Функция 1: f(x, y) = x^2 - 6x + y^2 - 8y
// df/dx = 2x - 6, df/dy = 2y - 8
double func1(double x, double y) {
    return x * x - 6 * x + y * y - 8 * y;
}

void grad1(double x, double y, double *gx, double *gy) {
    *gx = 2 * x - 6;
    *gy = 2 * y - 8;
}

// Функция 2: f(x, y) = x^2 - 6x + 2y^2 - 8y
// df/dx = 2x - 6, df/dy = 4y - 8
double func2(double x, double y) {
    return x * x - 6 * x + 2 * y * y - 8 * y;
}

void grad2(double x, double y, double *gx, double *gy) {
    *gx = 2 * x - 6;
    *gy = 4 * y - 8;
}

// Функция 3: f(x, y) = (x - 2)^2 + (x - y^2)^2
// df/dx = 2(x-2) + 2(x-y^2), df/dy = -4y(x-y^2)
double func3(double x, double y) {
    double t1 = (x - 2) * (x - 2);
    double t2 = (x - y * y) * (x - y * y);
    return t1 + t2;
}

void grad3(double x, double y, double *gx, double *gy) {
    double term = x - y * y;
    *gx = 2 * (x - 2) + 2 * term;
    *gy = -4 * y * term;
}

// ============================================================
// МЕТОД ГРАДИЕНТНОГО СПУСКА
// ============================================================

void gradient_descent(double (*f)(double, double),
                      void (*grad)(double, double, double*, double*),
                      double x_start, double y_start,
                      double alpha, double tol, int max_iter,
                      double *x_opt, double *y_opt, double *f_opt,
                      int *iter_count) {
    
    double x = x_start;
    double y = y_start;
    double gx, gy;
    
    printf("\n");
    printf("============================================================\n");
    printf("МЕТОД ГРАДИЕНТНОГО СПУСКА\n");
    printf("============================================================\n");
    printf("\n");
    printf("Начальная точка: (%.4f, %.4f)\n", x, y);
    printf("Скорость обучения: %.4f\n", alpha);
    printf("Точность: %.0e\n", tol);
    printf("\n");
    printf("------------------------------------------------------------\n");
    printf("Итерация |       x       |       y       |     f(x,y)    |    Градиент    |  Шаг\n");
    printf("------------------------------------------------------------\n");
    
    int iter = 0;
    double f_curr = f(x, y);
    
    printf("   %2d    | %12.8f | %12.8f | %12.8f |                |\n",
           iter, x, y, f_curr);
    
    while (iter < max_iter) {
        iter++;
        
        grad(x, y, &gx, &gy);
        
        double grad_norm = sqrt(gx * gx + gy * gy);
        if (grad_norm < tol) {
            break;
        }
        
        // Адаптивный шаг с backtracking
        double step = alpha;
        double x_new = x - step * gx;
        double y_new = y - step * gy;
        double f_new = f(x_new, y_new);
        
        int backtrack = 0;
        while (f_new > f_curr && step > 1e-12) {
            step *= 0.5;
            x_new = x - step * gx;
            y_new = y - step * gy;
            f_new = f(x_new, y_new);
            backtrack++;
        }
        
        printf("   %2d    | %12.8f | %12.8f | %12.8f | (%6.2f,%6.2f) | %8.6f",
               iter, x_new, y_new, f_new, gx, gy, step);
        if (backtrack > 0) {
            printf(" (x%d)", backtrack);
        }
        printf("\n");
        
        x = x_new;
        y = y_new;
        f_curr = f_new;
    }
    
    printf("------------------------------------------------------------\n");
    printf("Сошлись за %d итераций\n", iter);
    printf("\n");
    
    *x_opt = x;
    *y_opt = y;
    *f_opt = f_curr;
    *iter_count = iter;
}

// ============================================================
// ГЕНЕРАЦИЯ СЛУЧАЙНОЙ ТОЧКИ
// ============================================================

double random_in_range(double min, double max) {
    return min + (double)rand() / RAND_MAX * (max - min);
}

// ============================================================
// ОСНОВНАЯ ПРОГРАММА
// ============================================================

int main() {
    srand(time(NULL));
    
    double x_min = -10.0, x_max = 10.0;
    double y_min = -10.0, y_max = 10.0;
    
    double x_start = random_in_range(x_min, x_max);
    double y_start = random_in_range(y_min, y_max);
    
    // Скорость обучения подбирается под функцию
    double alpha = 0.1;
    double tol = 1e-9;
    int max_iter = 10000;
    
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
    void (*grad)(double, double, double*, double*);
    char *func_name;
    
    switch (choice) {
        case 1:
            func = func1;
            grad = grad1;
            func_name = "f(x, y) = x^2 - 6x + y^2 - 8y";
            alpha = 0.4;
            break;
        case 2:
            func = func2;
            grad = grad2;
            func_name = "f(x, y) = x^2 - 6x + 2y^2 - 8y";
            alpha = 0.3;
            break;
        case 3:
            func = func3;
            grad = grad3;
            func_name = "f(x, y) = (x - 2)^2 + (x - y^2)^2";
            alpha = 0.05;
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
    printf("Начальная точка: (%.4f, %.4f) (случайная)\n", x_start, y_start);
    printf("Скорость обучения: %.4f\n", alpha);
    printf("Точность: %.0e\n", tol);
    printf("\n");
    printf("Нажмите Enter для начала поиска...");
    getchar();
    
    gradient_descent(func, grad, x_start, y_start, alpha, tol, max_iter,
                     &x_opt, &y_opt, &f_opt, &iter_count);
    
    printf("\n");
    printf("============================================================\n");
    printf("РЕЗУЛЬТАТ\n");
    printf("============================================================\n");
    printf("\n");
    printf("  x* = %.10f\n", x_opt);
    printf("  y* = %.10f\n", y_opt);
    printf("  f(x*, y*) = %.10f\n", f_opt);
    printf("\n");
    printf("  Итераций: %d\n", iter_count);
    printf("\n");
    printf("============================================================\n");
    
    return 0;
}