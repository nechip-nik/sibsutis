#ifndef LIB_CRYP_H
#define LIB_CRYP_H
unsigned long mod_exp(unsigned long base, unsigned long exp, unsigned long mod);
long extended_gcd(long a, long b, long *x, long *y);
int discrete_log(int base, int target, int mod, int m, int k);
#endif