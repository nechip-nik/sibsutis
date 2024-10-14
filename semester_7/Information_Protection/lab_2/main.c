#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "crypto.h"  // Подключаем заголовочный файл с функциями шифрования

int main() {
    char a;
    if (scanf(" %c", &a) != 1) {
        printf("Ошибка ввода.\n");
        return 1;
    }

    switch (a) {
        case '1': {
            long p, m, cA, cB;

            p = 19;
            printf("Параметр p = %ld\n", p);

            cA = 5;
            printf("cA = %ld\n", cA);

            cB = 5;
            printf("cB = %ld\n", cB);

            printf("Введите сообщение m: ");
            if (scanf("%ld", &m) != 1) {
                printf("Ошибка ввода.\n");
                return 1;
            }

            shamir_cipher(p, m, cA, cB);
            break;
        }
        case '2': {
            long p, m, cB, k, g;

            p = 5;
            printf("Параметр p = %ld\n", p);

            g = 4;
            printf("Параметр g = %ld\n", g);

            cB = 5;
            printf("cB = %ld\n", cB);

            k = 2;
            printf("Параметр k = %ld\n", k); // Исправлено на k

            printf("Введите сообщение m: ");
            if (scanf("%ld", &m) != 1) {
                printf("Ошибка ввода.\n");
                return 1;
            }

            elgamal_cipher(p, g, m, cB, k);
            break;
        }
        case '3': {
            char message[] = "HELLOOO";
            char key[] = "XMCKLll";

            printf("Исходное сообщение: %s\n", message);

            // Шифрование
            vernam_cipher(message, key);
            printf("Зашифрованное сообщение: %s\n", message);

            // Дешифрование
            vernam_cipher(message, key);
            printf("Расшифрованное сообщение: %s\n", message);
            break;
        }
        case '4': {
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
                printf("%d ", encrypted_message[i]);
            }
            printf("\n");

            // Расшифрование
            char decrypted_message[len + 1];
            for (int i = 0; i < len; i++) {
                decrypted_message[i] = decrypt(encrypted_message[i], d, n);
            }
            decrypted_message[len] = '\0'; // Добавляем завершающий нулевой символ

            printf("Расшифрованное сообщение: %s\n", decrypted_message);
            break;
        }
        default:
            printf("Неверный выбор.\n");
            break;
    }
    return 0;
}