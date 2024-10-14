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
      unsigned long p, g;
      unsigned long a, b;
      unsigned long A, B;
      unsigned long secret_key_A, secret_key_B;

      p = 19;
      g = 2;

      a = 5;
      b = 7;

      A = mod_exp(g, a, p);
      B = mod_exp(g, b, p);

      secret_key_A = mod_exp(B, a, p);
      secret_key_B = mod_exp(A, b, p);

      if (secret_key_A == secret_key_B) {
        printf("Общий секретный ключ: %lu\n", secret_key_A);
        break;
      } else {
        printf("Ошибка: ключи не совпадают!\n");
        break;
      }
    }
    case '4': {
      int base = 23;
      int target = 55;
      int mod = 23;
      int m = 2;
      int k = 4;

      int result = discrete_log(base, target, mod, m, k);
      if (result != -1) {
        printf("Решение найдено: x = %d\n", result);
        break;
      } else {
        printf("Решение не найдено.\n");
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