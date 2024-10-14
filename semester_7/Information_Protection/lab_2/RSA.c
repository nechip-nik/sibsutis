#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypto.h"

// Функция для вычисления наибольшего общего делителя (НОД)
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

int main() {
    // Выбираем два простых числа
    int p = 61;
    int q = 53;

    // Вычисляем модуль n
    int n = p * q;

    // Вычисляем функцию Эйлера phi(n)
    int phi = (p - 1) * (q - 1);

    // Выбираем открытую экспоненту e
    int e = 17;

    // Проверяем, что e и phi взаимно просты
    if (gcd(e, phi) != 1) {
        printf("Ошибка: e и phi не взаимно просты.\n");
        return 1;
    }

    // Вычисляем секретную экспоненту d
    int d = mod_inverse(e, phi);

    // Сообщение для шифрования
    char message[] = "Аюша привет";
    int len = strlen(message);

    // Шифрование
    int encrypted_message[len];
    for (int i = 0; i < len; i++) {
        encrypted_message[i] = encrypt(message[i], e, n);
    }

    printf("Зашифрованное сообщение: ");
    for (int i = 0; i < len; i++) {
        printf("%c ", encrypted_message[i]);
    }
    printf("\n");

    // Расшифрование
    char decrypted_message[len];
    for (int i = 0; i < len; i++) {
        decrypted_message[i] = decrypt(encrypted_message[i], d, n);
    }
    decrypted_message[len] = '\0'; // Добавляем завершающий нулевой символ

    printf("Расшифрованное сообщение: %s\n", decrypted_message);

    return 0;
}