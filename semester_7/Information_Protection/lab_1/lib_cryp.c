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
  if (a == 0) {
    *x = 0;
    *y = 1;
    return b;
  }

  long x1, y1;
  long gcd = extended_gcd(b % a, a, &x1, &y1);

  *x = y1 - (b / a) * x1;
  *y = x1;

  return gcd;
}
int discrete_log(int base, int target, int mod, int m, int k) {
  int n = (int)sqrt(mod) + 1;
  int giant_step = mod_exp(base, n, mod);

  int *baby_steps = (int *)malloc(sizeof(int) * m);
  for (int i = 0; i < m; i++) {
    baby_steps[i] = -1;
  }

  int current = target;
  for (int j = 0; j < n; j++) {
    for (int i = 0; i < m; i++) {
      if (baby_steps[i] == -1) {
        baby_steps[i] = current;
        break;
      } else if (baby_steps[i] == current) {
        break;
      }
    }
    current = (current * base) % mod;
  }

  current = 1;
  for (int i = 0; i < n; i++) {
    int giant_step_value = current;
    for (int j = 0; j < m; j++) {
      if (baby_steps[j] == giant_step_value) {
        free(baby_steps);
        return i * n + j;
      }
    }
    current = (current * giant_step) % mod;
  }

  free(baby_steps);
  return -1;
}