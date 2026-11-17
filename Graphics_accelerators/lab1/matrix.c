#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>

static long perf_event_open(struct perf_event_attr *hw, pid_t pid,
                            int cpu, int group_fd, unsigned long flags) {
    return syscall(__NR_perf_event_open, hw, pid, cpu, group_fd, flags);
}

static int open_cycles(void) {
    struct perf_event_attr pe;
    memset(&pe, 0, sizeof(pe));
    pe.type = PERF_TYPE_HARDWARE;
    pe.size = sizeof(pe);
    pe.config = PERF_COUNT_HW_CPU_CYCLES;
    pe.disabled = 1;
    pe.exclude_kernel = 1;
    pe.exclude_hv = 1;
    int fd = perf_event_open(&pe, 0, -1, -1, 0);
    if (fd == -1) { perror("perf_event_open"); exit(1); }
    return fd;
}

#define REPS 10

typedef void (*mul_fn)(double*, double*, double*, int, int, int);

static long long measure(int fd, mul_fn fn,
                         double *X, double *Y, double *Z, int n, int m, int r) {
    long long best = -1;
    for (int rep = 0; rep < REPS; rep++) {
        ioctl(fd, PERF_EVENT_IOC_RESET, 0);
        ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);

        fn(X, Y, Z, n, m, r);

        ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
        long long c = 0;
        if (read(fd, &c, sizeof(c)) == sizeof(c)) {
            if (best < 0 || c < best) best = c;
        }
    }
    return best;
}

// линейный
static void mul_naive(double *X, double *Y, double *Z, int n, int m, int r) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < r; j++) {
            double sum = 0.0;
            for (int k = 0; k < m; k++)
                sum += X[i*m+k] * Y[k*r+j];
            Z[i*r+j] = sum;
        }
}


// транспонирование
static void mul_transposed(double *X, double *Y, double *Z, int n, int m, int r) {
    double *Yt = malloc(sizeof(double) * m * r);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < r; j++)
            Yt[j*m+i] = Y[i*r+j];

    for (int i = 0; i < n; i++) {
        double *Xrow = X + i*m;
        double *Zrow = Z + i*r;
        for (int j = 0; j < r; j++) {
            double *Ytrow = Yt + j*m;
            double sum = 0.0;
            for (int k = 0; k < m; k++)
                sum += Xrow[k] * Ytrow[k];
            Zrow[j] = sum;
        }
    }
    free(Yt);
}


int main(void) {
    int n = 256, m = 256, r = 256;

    double *X = malloc(sizeof(double) * n * m);
    double *Y = malloc(sizeof(double) * m * r);
    double *Z = malloc(sizeof(double) * n * r);
    if (!X || !Y || !Z) { perror("malloc"); return 1; }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            X[i*m+j] = 2.0;

    for (int i = 0; i < m; i++)
        for (int j = 0; j < r; j++)
            Y[i*r+j] = (double)j;

    int fd = open_cycles();

    mul_naive(X, Y, Z, n, m, r);
    mul_naive(X, Y, Z, n, m, r);

    long long t1 = measure(fd, mul_naive,      X, Y, Z, n, m, r);
    long long t2 = measure(fd, mul_transposed, X, Y, Z, n, m, r);

    printf("Матрицы %dx%dx%d\n\n", n, m, r);
    printf("1. Наивный i-j-k:                    %12lld тактов\n", t1);
    printf("2. Транспонирование:         %12lld  x%.2f\n", t2, (double)t1/t2);

    close(fd);
    free(X); free(Y); free(Z);
    return 0;
}