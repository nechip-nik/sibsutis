#ifndef CRYPTO_H
#define CRYPTO_H
char xor_char(char a, char b);
void vernam_cipher(char *message, char *key);
long power_modulo(long base, long exp, long mod);
int gcd(int a, int b);
int mod_inverse(int e, int phi);
int encrypt(int m, int e, int n);
int decrypt(int c, int d, int n);
void shamir_cipher(long p, long m, long cA, long cB);
void elgamal_cipher(long p, long g, long m, long cB, long k);

#endif // CRYPTO_H