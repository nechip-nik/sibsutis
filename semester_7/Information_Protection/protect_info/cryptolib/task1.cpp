#include "task1.h"

long power_modulo(long a, long x, long p) // a - основание степени
// x - показатель степени
// p - модуль
{
    long y = 1;
    long s = a;

    std::bitset<64> bin_x(x);

    for (int i = 0; i < 64; ++i)
    {
        if (bin_x[i] == 1)
        {
            y = (y * s) % p;
        }

        s = (s * s) % p;
    }

    return y;
}

void gen_euclid(long a, long b, long &gcd, long &x, long &y)
{
    long U[3] = {a, 1, 0};
    long V[3] = {b, 0, 1};

    while (V[0] != 0)
    {
        long q = U[0] / V[0];

        long T[3] = {U[0] % V[0], U[1] - q * V[1], U[2] - q * V[2]};

        for (int i = 0; i < 3; ++i)
        {
            U[i] = V[i];
        }

        for (int i = 0; i < 3; ++i)
        {
            V[i] = T[i];
        }
    }

    gcd = U[0];
    x = U[1];
    y = U[2];
}

bool miller_rabin_test(long long n, int k)
{
    if (n <= 1 || n == 4)
        return false;
    if (n <= 3)
        return true;

    // Находим d такое, что n - 1 = d * 2^r
    long d = n - 1;
    while (d % 2 == 0)
        d /= 2;

    // Выполняем тест Миллера-Рабина k раз
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long> dis(2, n - 2);

    for (int i = 0; i < k; ++i)
    {
        long a = dis(gen);
        long x = power_modulo(a, d, n);

        if (x == 1 || x == n - 1)
            continue;

        bool continue_outer = false;
        for (long r = 1; r < d; ++r)
        {
            x = (x * x) % n;
            if (x == 1)
                return false;
            if (x == n - 1)
            {
                continue_outer = true;
                break;
            }
        }
        if (continue_outer)
            continue;

        return false;
    }

    return true;
}

long random_generator_simple(long min, long max)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(min, max);

    while (true)
    {
        long candidate = dis(gen);
        if (candidate % 2 == 0)
        {
            candidate++;
        }

        if (miller_rabin_test(candidate, 10))
        {
            return candidate;
        }
    }

    // return dis(gen);
}

long random_generator(long min, long max)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(min, max);

    return dis(gen);
}

long generate_coprime(long p)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long> dis(2, p - 1);

    while (true)
    {
        long candidate = dis(gen);
        if (std::gcd(candidate, p) == 1)
        {
            return candidate;
        }
    }
}

void diffie_hellman(long p, long g)
{
    long X_a = random_generator(1, p - 1);
    long X_b = random_generator(1, p - 1); // секретные ключи абонентов A и B

    long Y_a = power_modulo(g, X_a, p);
    long Y_b = power_modulo(g, X_a, p);

    long Z_a = power_modulo(Y_b, X_a, p);
    long Z_b = power_modulo(Y_a, X_b, p);

    if (Z_a == Z_b)
    {
        std::cout << "Общий секретный ключ для A и B: " << Z_a << std::endl;
    }
    else
    {
        std::cout << "Ошибка: ключи не совпадают" << std::endl;
    }
}

long baby_giant(long a, long y, long p)
{
    long m = static_cast<long>(sqrt(p)) + 1;
    std::vector<long> baby_steps(m, -1);

    long a_inv = power_modulo(a, p - 2, p);

    long curr = y;

    for (auto j = 0; j < m; ++j)
    {
        baby_steps[j] = curr;
        curr = (curr * a_inv) % p;
    }

    long am = power_modulo(a, m, p);

    curr = 1;
    for (auto j = 0; j < m; ++j)
    {
        for (auto i = 0; i < m; ++i)
        {
            if (baby_steps[i] == curr)
            {
                return (j * m) + i;
            }
        }
        curr = (curr * am) % p;
    }

    return -1;
}
