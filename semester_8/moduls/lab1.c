#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Константы a и b
const double a = 1.0;
const double b = -1.152 ;

// Функция для вычисления плотности распределения f(x)
double f(double x) {
    if (-0.2 <= x && x < 1) {
        return a * pow(x + 0.2, 3); // (x + 0.2)^3
    } else if (1 <= x && x <= 2.5) {
        return b * (x - 2.5);
    } else {
        return 0.0;
    }
}

// Функция для генерации случайного числа с заданной плотностью распределения
double generate_random() {
    while (1) {
        // Генерация случайного числа x в интервале [-0.2, 2.5]
        double x = -0.2 + (double)rand() / RAND_MAX * (2.5 - (-0.2));
        
        // Генерация случайного числа y в интервале [0, 0.952]
        double y = (double)rand() / RAND_MAX * 0.952;
        
        // Проверка условия принятия x
        if (y <= f(x)) {
            return x;
        }
    }
}

int main() {
    // Инициализация генератора случайных чисел
    srand(time(NULL));

    // Генерация 10 случайных чисел
    double random_numbers[1000];
    for (int i = 0; i < 1000; i++) {
        random_numbers[i] = generate_random();
    }

    // Вывод сгенерированных чисел
    printf("Сгенерированные числа:\n");
    for (int i = 0; i < 1000; i++) {
        printf("%f\n", random_numbers[i]);
    }

    return 0;
}