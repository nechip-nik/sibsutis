#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define N 100000000

static inline uint64_t rdtsc(void) {
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

// ---------- INTEGER ----------

double lat_add_i64(void) {
    uint64_t a = 1, b = 3;
    uint64_t t0 = rdtsc();
    for (uint64_t i = 0; i < N; i++) {
        a = a + b;  
    }
    uint64_t t1 = rdtsc();
    volatile uint64_t sink = a; (void)sink;
    return (double)(t1 - t0) / N;
}

double tp_add_i64(void) {
    uint64_t a0=1,a1=2,a2=3,a3=4;
    const uint64_t b0=5,b1=6,b2=7,b3=8;
    uint64_t t0 = rdtsc();
    for (uint64_t i = 0; i < N; i++) {
        a0 += b0; a1 += b1; a2 += b2; a3 += b3;
    }
    uint64_t t1 = rdtsc();
    volatile uint64_t sink = a0+a1+a2+a3; (void)sink;
    return (double)(t1 - t0) / (N * 4);
}

double lat_mul_i64(void) {
    uint64_t a = 3, b = 5;
    uint64_t t0 = rdtsc();
    for (uint64_t i = 0; i < N; i++) {
        a = a * b;
    }
    uint64_t t1 = rdtsc();
    volatile uint64_t sink = a; (void)sink;
    return (double)(t1 - t0) / N;
}

double tp_mul_i64(void) {
    uint64_t a0=3,a1=5,a2=7,a3=11;
    const uint64_t b0=2,b1=3,b2=5,b3=7;
    uint64_t t0 = rdtsc();
    for (uint64_t i = 0; i < N; i++) {
        a0 *= b0; a1 *= b1; a2 *= b2; a3 *= b3;
    }
    uint64_t t1 = rdtsc();
    volatile uint64_t sink = a0+a1+a2+a3; (void)sink;
    return (double)(t1 - t0) / (N * 4);
}

// ---------- FLOATING POINT ----------

double lat_add_f64(void) {
    double a = 1.0, b = 1.0000001;
    uint64_t t0 = rdtsc();
    for (uint64_t i = 0; i < N; i++) {
        a = a + b;
    }
    uint64_t t1 = rdtsc();
    volatile double sink = a; (void)sink;
    return (double)(t1 - t0) / N;
}

double tp_add_f64(void) {
    double a0=1,a1=2,a2=3,a3=4;
    const double b=1.0000001;
    uint64_t t0 = rdtsc();
    for (uint64_t i = 0; i < N; i++) {
        a0 += b; a1 += b; a2 += b; a3 += b;
    }
    uint64_t t1 = rdtsc();
    volatile double sink = a0+a1+a2+a3; (void)sink;
    return (double)(t1 - t0) / (N * 4);
}

double lat_mul_f64(void) {
    double a = 1.0001, b = 1.0000001;
    uint64_t t0 = rdtsc();
    for (uint64_t i = 0; i < N; i++) {
        a = a * b;
    }
    uint64_t t1 = rdtsc();
    volatile double sink = a; (void)sink;
    return (double)(t1 - t0) / N;
}

double tp_mul_f64(void) {
    double a0=1.1,a1=1.2,a2=1.3,a3=1.4;
    const double b=1.0000001;
    uint64_t t0 = rdtsc();
    for (uint64_t i = 0; i < N; i++) {
        a0 *= b; a1 *= b; a2 *= b; a3 *= b;
    }
    uint64_t t1 = rdtsc();
    volatile double sink = a0+a1+a2+a3; (void)sink;
    return (double)(t1 - t0) / (N * 4);
}

double lat_fma_f64(void) {
    double a = 1.0, b = 1.0000001, c = 0.0000001;
    uint64_t t0 = rdtsc();
    for (uint64_t i = 0; i < N; i++) {
        a = a * b + c;
    }
    uint64_t t1 = rdtsc();
    volatile double sink = a; (void)sink;
    return (double)(t1 - t0) / N;
}

double tp_fma_f64(void) {
    double a0=1.0,a1=1.1,a2=1.2,a3=1.3;
    const double b=1.0000001, c=0.0000001;
    uint64_t t0 = rdtsc();
    for (uint64_t i = 0; i < N; i++) {
        a0 = a0*b + c;
        a1 = a1*b + c;
        a2 = a2*b + c;
        a3 = a3*b + c;
    }
    uint64_t t1 = rdtsc();
    volatile double sink = a0+a1+a2+a3; (void)sink;
    return (double)(t1 - t0) / (N * 4);
}
// ---------- MATRIX ----------
// static void mul_naive(double *X, double *Y, double *Z, int n, int m, int r) {
//     for (int i = 0; i < n; i++)
//         for (int j = 0; j < r; j++) {
//             double sum = 0.0;
//             for (int k = 0; k < m; k++)
//                 sum += X[i*m+k] * Y[k*r+j];
//             Z[i*r+j] = sum;
//         }
// }
// ---------- OUTPUT ----------

static void report(const char *name, const char *kind, double cycles) {
    printf("  %-28s %-10s : %6.3f\n", name, kind, cycles);
}

int main(void) {
    printf("N = %llu итераций на тест\n", (unsigned long long)N);


    printf("Целочисленные:\n");
    report("ADD (int64)", "latency",    lat_add_i64());
    report("ADD (int64)", "throughput", tp_add_i64());
    report("MUL (int64)", "latency",    lat_mul_i64());
    report("MUL (int64)", "throughput", tp_mul_i64());

    printf("\nС плавающей точкой (double):\n");
    report("ADD (f64)",   "latency",    lat_add_f64());
    report("ADD (f64)",   "throughput", tp_add_f64());
    report("MUL (f64)",   "latency",    lat_mul_f64());
    report("MUL (f64)",   "throughput", tp_mul_f64());
    report("FMA (f64)",   "latency",    lat_fma_f64());
    report("FMA (f64)",   "throughput", tp_fma_f64());

    return 0;
}