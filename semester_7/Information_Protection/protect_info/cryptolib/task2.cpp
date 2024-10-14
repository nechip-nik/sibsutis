#include "task2.h"

long mod_inverse(long c, long p_minus1)
{
    long gcd, x, y;
    gen_euclid(c, p_minus1, gcd, x, y);

    if (gcd != 1)
    {
        throw std::invalid_argument("Нет мультипликативного обратного, так как c и p_minus1 не взаимно просты.");
    }

    // x может быть отрицательным, поэтому приводим его к положительному значению
    if (x < 0)
    {
        x += p_minus1;
    }

    return x;
}

void shamir_cipher(long p, long m, long cA, long cB)
{
    long dA = mod_inverse(cA, p - 1);
    long dB = mod_inverse(cB, p - 1);

    // if ((power_modulo(cA * dA, 1, p - 1)) != 1 || power_modulo(cB * dB, 1, p - 1) != 1)
    // {
    //     throw std::invalid_argument("хуйня переделывай.");
    // }

    long x1 = power_modulo(m, cA, p);
    long x2 = power_modulo(x1, cB, p);
    long x3 = power_modulo(x2, dA, p);
    long x4 = power_modulo(x3, dB, p);

    // long x4 = power_modulo(m, (cA * dA * cB * dB) % (p - 1), p);

    if (x4 == m)
    {
        std::cout << "Сообщение успешно передано и расшифровано: " << x4 << std::endl;
    }
    else
    {
        std::cout << "Ошибка при передаче сообщения, x4 = " << x4 << std::endl;
    }
}

void elgamal_cipher(long p, long g, long m, long cB, long k)
{
    long dB = power_modulo(g, cB, p);
    std::cout << "dB = " << dB << std::endl;

    long r = power_modulo(g, k, p);
    std::cout << "r = " << r << std::endl;

    // long e = (m * static_cast<long>(pow(dB, k))) % p;
    long e = power_modulo(m * (power_modulo(dB, k, p)), 1, p);
    std::cout << "e = " << e << std::endl;

    long m_hatch = power_modulo(e * (power_modulo(r, p - 1 - cB, p)), 1, p);
    std::cout << "m_hatch = " << m_hatch << std::endl;

    if (m == m_hatch)
    {
        std::cout << "Сообщение успешно передано и расшифровано: " << m_hatch << std::endl;
    }
    else
    {
        std::cout << "Ошибка при передаче сообщения" << std::endl;
    }
}

void rsa_cipher(long N, long m, long dB, long cB)
{
    long e, m_hatch;

    e = power_modulo(m, dB, N);
    m_hatch = power_modulo(e, cB, N);

    if (m == m_hatch)
    {
        std::cout << "Сообщение успешно передано и расшифровано: " << m_hatch << std::endl;
    }
    else
    {
        std::cout << "Ошибка при передаче сообщения" << std::endl;
    }
}

void vernam_cipher()