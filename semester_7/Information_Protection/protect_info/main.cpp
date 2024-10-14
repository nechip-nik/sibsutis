#include "task1.h"
#include "task2.h"

void print_using_task1()
{
    std::cout << "Выберите функцию для вызова\n"
              << std::endl;
    std::cout << "1) Быстрое возведение в степень по модулю" << std::endl;
    std::cout << "2) Обобщенный алгоритм Евклида" << std::endl;
    std::cout << "3) Схема Диффи-Хеллмана" << std::endl;
    std::cout << "4) Шаг младенца, шаг великана" << std::endl;
}

void selected_task1(int choice)
{
    switch (choice)
    {
    case 1:
    {
        long a, x, p;

        std::cout << "Введите основание степени (a): ";
        std::cin >> a;
        std::cout << "Введите показатель степени (x): ";
        std::cin >> x;
        std::cout << "Введите модуль (p): ";
        std::cin >> p;

        long result = power_modulo(a, x, p);

        std::cout << "Результат: " << a << "^" << x << " mod " << p << " = " << result << std::endl;
    }
    break;

    case 2:
    {
        long a, b;
        std::cout << "Введите два числа (a >= b): ";
        std::cin >> a >> b;

        long gcd, x, y;
        gen_euclid(a, b, gcd, x, y);

        std::cout << "НОД(" << a << ", " << b << ") = " << gcd << std::endl;
        std::cout << "Коэффициенты x и y: " << x << ", " << y << std::endl;
    }
    break;

    case 3:
    {
        long p, g;
        std::cout << "Введите числа p и g (1 < g < p -1): ";
        std::cin >> p >> g;

        diffie_hellman(p, g);
    }
    break;

    case 4:
    {
        long a, y, p;
        std::cout << "Введите числа a, y, p: ";
        std::cin >> a >> y >> p;

        long result = baby_giant(a, y, p);

        if (result != -1)
        {
            std::cout << "Дискретный логарифм: " << result << std::endl;
        }
        else
        {
            std::cout << "Дискретный логарифм не найден." << std::endl;
        }
    }

    default:
        break;
    }
}

void print_using_task2()
{
    std::cout << "Выберите функцию для вызова\n"
              << std::endl;
    std::cout << "1) Шифр Шамира" << std::endl;
    std::cout << "2) Шифр Эль-Гамаля" << std::endl;
    std::cout << "3) Шифр Вернама" << std::endl;
    std::cout << "4) Шифр RSA" << std::endl;
}

void selected_task2(int choice)
{
    switch (choice)
    {
    case (1):
    {
        long p, m, cA, cB;

        p = random_generator_simple(100000, 1000000);
        std::cout << "Параметр p = " << p << std::endl;

        cA = generate_coprime(p - 1);
        std::cout << "cA = " << cA << std::endl;

        cB = generate_coprime(p - 1);
        std::cout << "cB = " << cB << std::endl;

        std::cout << "Введите сообщение m: ";
        std::cin >> m;

        shamir_cipher(p, m, cA, cB);
    }
    break;
    case (2):
    {
        long p, m, cB, k, g;

        p = random_generator_simple(100000, 1000000);
        std::cout << "Параметр p = " << p << std::endl;

        // std::cin >> g;
        g = random_generator(100000, 1000000);
        std::cout << "Параметр g = " << g << std::endl;

        cB = random_generator(1, p - 1);
        std::cout << "Параметр cB = " << cB << std::endl;

        k = random_generator(1, p - 2);
        std::cout << "Параметр k = " << k << std::endl;

        std::cout << "Введите сообщение m: ";
        std::cin >> m;
        elgamal_cipher(p, g, m, cB, k);
    }
    break;
    case (3):
    {
        long m, k;
        
    }
    case (4):
    {
        long N, P, Q, m, phi, dB, cB;
        P = random_generator_simple(10000, 100000);
        std::cout << "Параметр P = " << P << std::endl;

        Q = random_generator_simple(10000, 100000);
        std::cout << "Параметр Q = " << Q << std::endl;

        N = P * Q;
        std::cout << "Параметр N = " << N << std::endl;

        phi = (P - 1) * (Q - 1);
        std::cout << "Параметр phi = " << phi << std::endl;

        dB = generate_coprime(phi);
        std::cout << "Параметр dB = " << dB << std::endl;

        cB = mod_inverse(dB, phi);
        std::cout
            << "Введите сообщение m: ";
        std::cin >> m;

        rsa_cipher(N, m, dB, cB);
    }
    case (5):
        exit(EXIT_SUCCESS);

    default:
        break;
    }
}

int main()
{
    int choice_task;
    int choice;
    std::cout << "Введите номер лабораторной работы: " << std::endl;
    std::cin >> choice_task;

    switch (choice_task)
    {
    case (1):
    {
        print_using_task1();
        std::cin >> choice;
        selected_task1(choice);
    }
    break;

    case (2):
    {
        print_using_task2();
        std::cin >> choice;
        selected_task2(choice);
    }

    default:
        break;
    }

    // print_using_task1();
    // std::cin >> choice;

    return 0;
}