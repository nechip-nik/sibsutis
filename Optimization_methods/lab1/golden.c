#include <stdio.h>
#include <math.h>

#define GOLDEN_RATIO_SQ ((3.0 - sqrt(5.0)) / 2.0)

double compute(double x, double y, int flag) {
    if (flag == 0) {
        // Одномерная функция: f(x) = x^2 - 6x
        return x * x - 6 * x;
    } else {
        // Двумерная функция: f(x, y) = x^2 - 6x + y^2 - 8y
        return x * x - 6 * x + y * y - 8 * y;
    }
}

// Метод золотого сечения для одномерной функции
double golden_section_search(double a, double b, double tol, int max_iter, 
                             int *iter_count, int flag) {
    double gr = GOLDEN_RATIO_SQ;
    
    // Начальные точки
    double x1 = a + gr * (b - a);
    double x2 = b - gr * (b - a);
    
    // Вычисляем значения через единую функцию
    double f1 = compute(x1, 0, flag);
    double f2 = compute(x2, 0, flag);
    
    int iter = 0;
    
    printf("\n");
    printf("+--------+------------------+------------------+------------------+------------------+------------------+------------------+\n");
    printf("| Итерац |        a         |        b         |       x1         |       x2         |     f(x1)        |     f(x2)        |\n");
    printf("+--------+------------------+------------------+------------------+------------------+------------------+------------------+\n");
    
    printf("|  %4d  | %16.8f | %16.8f | %16.8f | %16.8f | %16.8f | %16.8f |\n", 
           iter, a, b, x1, x2, f1, f2);
    printf("+--------+------------------+------------------+------------------+------------------+------------------+------------------+\n");
    
    while ((b - a) > tol && iter < max_iter) {
        iter++;
        
        if (f1 < f2) {
            b = x2;
            x2 = x1;
            f2 = f1;
            x1 = a + gr * (b - a);
            f1 = compute(x1, 0, flag);
        } else {
            a = x1;
            x1 = x2;
            f1 = f2;
            x2 = b - gr * (b - a);
            f2 = compute(x2, 0, flag);
        }
        
        printf("|  %4d  | %16.8f | %16.8f | %16.8f | %16.8f | %16.8f | %16.8f |\n", 
               iter, a, b, x1, x2, f1, f2);
        printf("+--------+------------------+------------------+------------------+------------------+------------------+------------------+\n");
    }
    
    *iter_count = iter;
    return (a + b) / 2;
}

int main() {
    double a = -10.0, b = 10.0;
    double tol = 1e-6;
    int max_iter = 1000;
    int iter_count;
    
    printf("========================================\n");
    printf("МЕТОД ЗОЛОТОГО СЕЧЕНИЯ\n");
    printf("========================================\n");
    printf("\n");
    printf("Функция: f(x) = x^2 - 6x\n");
    printf("Интервал: [%.1f, %.1f]\n", a, b);
    printf("Точность: %.0e\n", tol);
    printf("\n");
    printf("  • a, b   - границы текущего интервала\n");
    printf("  • x1, x2 - внутренние точки (x1 < x2)\n");
    printf("  • f(x1), f(x2) - значения функции в этих точках\n");
    printf("\n");
    printf("Нажмите Enter для начала...");
    getchar();
    
    double x_min = golden_section_search(a, b, tol, max_iter, &iter_count, 0);
    double f_min = compute(x_min, 0, 0);
    
    printf("\n");

    printf("  Найденный минимум:  x = %.10f\n", x_min);
    printf("  Значение функции:   f(x) = %.10f\n", f_min);
    printf("  Количество итераций: %d\n", iter_count);
    
    return 0;
}