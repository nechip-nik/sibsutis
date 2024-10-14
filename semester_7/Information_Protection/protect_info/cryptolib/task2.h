#pragma once

#include "task1.h"
#include <unistd.h>

long mod_inverse(long c, long p_minus1);
void shamir_cipher(long p, long m, long cA, long cB);

void elgamal_cipher(long p, long g, long m, long cB, long k);
void vernam_cipher();
void rsa_cipher(long N, long m, long dB, long cB);