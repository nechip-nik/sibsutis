#pragma once

#include <cmath>
#include <iostream>
#include <bitset>
#include <random>
#include <stdexcept>

long power_modulo(long a, long n, long m);
void gen_euclid(long a, long b, long &gcd, long &x, long &y);
void diffie_hellman(long p, long g);
long baby_giant(long a, long y, long p);

bool miller_rabin_test(long long n, int k);
long random_generator(long min, long max);
long random_generator_simple(long min, long max);
long generate_coprime(long p);
long mod_exp(long g, long exp, long p);