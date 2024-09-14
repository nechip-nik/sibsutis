#ifndef LIB_CRYP_H
#define LIB_CRYP_H
unsigned long mod_exp(unsigned long base, unsigned long exp, unsigned long mod);
long extended_gcd(long a, long b, long *x, long *y);
unsigned long long baby_step_giant_step(unsigned long long g,
                                        unsigned long long h,
                                        unsigned long long p);
#endif