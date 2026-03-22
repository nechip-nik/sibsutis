#ifndef RDTSC_H
#define RDTSC_H

#ifdef __GNUC__
static inline unsigned long long rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}
#else
#include <intrin.h>
#pragma intrinsic(__rdtsc)
static inline unsigned long long rdtsc() {
    return __rdtsc();
}
#endif

static inline unsigned int CC() {
    return (unsigned int)(rdtsc() & 0xFFFFFFFF);
}

#endif // RDTSC_H