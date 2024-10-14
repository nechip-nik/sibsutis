#include "crypto.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

char xor_char(char a, char b) {
    return a ^ b;
}

// Функция для шифрования/дешифрования сообщения с использованием ключа
void vernam_cipher(char *message, char *key) {
    int len = strlen(message);
    for (int i = 0; i < len; i++) {
        message[i] = xor_char(message[i], key[i]);
    }
}
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Функция для вычисления мультипликативного обратного по модулю
int mod_inverse(int e, int phi) {
    int d;
    for (d = 1; d < phi; d++) {
        if ((e * d) % phi == 1) {
            return d;
        }
    }
    return -1; // Если обратный элемент не найден
}

// Функция для шифрования
int encrypt(int m, int e, int n) {
    int c = 1;
    for (int i = 0; i < e; i++) {
        c = (c * m) % n;
    }
    return c;
}

// Функция для расшифрования
int decrypt(int c, int d, int n) {
    int m = 1;
    for (int i = 0; i < d; i++) {
        m = (m * c) % n;
    }
    return m;
}
long power_modulo(long base, long exp, long mod) {
    long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

void shamir_cipher(long p, long m, long cA, long cB) {
    long dA = mod_inverse(cA, p - 1);
    long dB = mod_inverse(cB, p - 1);

    long x1 = power_modulo(m, cA, p);
    long x2 = power_modulo(x1, cB, p);
    long x3 = power_modulo(x2, dA, p);
    long x4 = power_modulo(x3, dB, p);

    if (x4 == m) {
        printf("Сообщение успешно передано и расшифровано: %ld\n", x4);
    } else {
        printf("Ошибка при передаче сообщения, x4 = %ld\n", x4);
    }
}

void elgamal_cipher(long p, long g, long m, long cB, long k) {
    long dB = power_modulo(g, cB, p);
    printf("dB = %ld\n", dB);

    long r = power_modulo(g, k, p);
    printf("r = %ld\n", r);

    long e = power_modulo(m * power_modulo(dB, k, p), 1, p);
    printf("e = %ld\n", e);

    long m_hatch = power_modulo(e * power_modulo(r, p - 1 - cB, p), 1, p);
    printf("m_hatch = %ld\n", m_hatch);

    if (m == m_hatch) {
        printf("Сообщение успешно передано и расшифровано: %ld\n", m_hatch);
    } else {
        printf("Ошибка при передаче сообщения\n");
    }
}