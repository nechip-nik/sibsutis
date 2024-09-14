#include "lib_cryp.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
unsigned long mod_exp(unsigned long base, unsigned long exp,
                      unsigned long mod) {
  unsigned long result = 1;
  base = base % mod; // Уменьшаем base, чтобы избежать переполнения

  while (exp > 0) {
    // Если exp нечетное, умножаем result на base и берем по модулю
    if (exp % 2 == 1) {
      result = (result * base) % mod;
    }

    // exp теперь четное
    exp = exp >> 1;             // exp = exp / 2
    base = (base * base) % mod; // base = base^2 % mod
  }

  return result;
}
long extended_gcd(long a, long b, long *x, long *y) {
  // Базовый случай
  if (a == 0) {
    *x = 0;
    *y = 1;
    return b;
  }

  // Переменные для хранения результатов рекурсивного вызова
  long x1, y1;
  long gcd = extended_gcd(b % a, a, &x1, &y1);

  // Обновляем x и y, используя результаты рекурсивного вызова
  *x = y1 - (b / a) * x1;
  *y = x1;

  return gcd;
}
unsigned long long baby_step_giant_step(unsigned long long g,
                                        unsigned long long h,
                                        unsigned long long p) {
  unsigned long long m = (unsigned long long)sqrt(p) + 1;
  unsigned long long g_inv =
      mod_exp(g, p - 2, p); // Вычисляем обратный элемент g по модулю p

  // Создаем таблицу для хранения значений (g^j, j) для j от 0 до m-1
  unsigned long long *baby_steps =
      (unsigned long long *)malloc(m * sizeof(unsigned long long));
  for (unsigned long long j = 0; j < m; j++) {
    baby_steps[j] = mod_exp(g, j, p);
  }

  // Вычисляем g^(-m)
  unsigned long long g_inv_m = mod_exp(g_inv, m, p);

  // Ищем совпадение в таблице
  unsigned long long h_i = h;
  for (unsigned long long i = 0; i < m; i++) {
    // Проверяем, есть ли h_i в таблице baby_steps
    for (unsigned long long j = 0; j < m; j++) {
      if (baby_steps[j] == h_i) {
        free(baby_steps);
        return i * m + j;
      }
    }
    // Обновляем h_i
    h_i = (h_i * g_inv_m) % p;
  }

  free(baby_steps);
  return -1;
}