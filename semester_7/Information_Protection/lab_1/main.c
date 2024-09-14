#include "lib_cryp.h"
#include <stdio.h>

int main() {
  char key;
  while (1) {
    printf("Введите команду (q - выйти): ");
    scanf(" %c", &key);

    switch (key) {
    case '1': {
      unsigned long base, exp, mod;
      printf("Введите base, exp, mod: ");
      if (scanf("%lu %lu %lu", &base, &exp, &mod) != 3) {
        printf("Ошибка ввода\n");
        while (getchar() != '\n')
          ;
        break;
      }

      unsigned long buffer = mod_exp(base, exp, mod);
      printf("Результат: %lu\n", buffer);
      break;
    }
    case '2': {
      long a, b, x, y;
      printf("Введите a, b: ");
      if (scanf("%lu %lu", &a, &b) != 2) {
        printf("Ошибка ввода\n");
        while (getchar() != '\n')
          ;
        break;
      }

      long buffer = extended_gcd(a, b, &x, &y);
      printf("Результат: %ld,    x=%ld,     y=%ld\n", buffer, x, y);
      break;
    }
    case '3': {
      unsigned long p, g; // Общедоступные (публичные) параметры
      unsigned long a, b; // Секретные ключи абонентов
      unsigned long A, B; // Открытые ключи абонентов
      unsigned long secret_key_A, secret_key_B; // Общие секретные ключи

      // Выбираем большое простое число p и генератор g
      p = 19; // Пример простого числа
      g = 2;  // Пример генератора

      // Выбираем секретные ключи для каждого абонента
      a = 5; // Секретный ключ абонента A
      b = 7; // Секретный ключ абонента B

      // Вычисляем открытые ключи для каждого абонента
      A = mod_exp(g, a, p); // Открытый ключ абонента A
      B = mod_exp(g, b, p); // Открытый ключ абонента B

      // Вычисляем общие секретные ключи
      secret_key_A = mod_exp(B, a, p); // Общий ключ для абонента A
      secret_key_B = mod_exp(A, b, p); // Общий ключ для абонента B

      // Проверяем, что оба абонента получили одинаковый ключ
      if (secret_key_A == secret_key_B) {
        printf("Общий секретный ключ: %lu\n", secret_key_A);
        break;
      } else {
        printf("Ошибка: ключи не совпадают!\n");
        break;
      }
    }
    case '4': {
      unsigned long long g, h, p;
      printf("Введите g, h, p: ");
      if (scanf("%llu %llu %llu", &g, &h, &p) != 3) {
        printf("Ошибка ввода\n");
        return 1;
      }

      unsigned long long result = baby_step_giant_step(g, h, p);
      if (result != -1) {
        printf("Дискретный логарифм: %llu\n", result);
        break;
      } else {
        printf("Дискретный логарифм не найден\n");
        break;
      }
    }
    case 'q':
      printf("Выход из программы\n");
      return 0;
    default:
      printf("Неизвестная команда\n");
      break;
    }
  }
}