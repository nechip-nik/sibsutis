#include <stdio.h>
#include <math.h>

// Функция для расчета факториала
double factorial(int n)
{
    double result = 1.0;
    for (int i = 1; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

// Функция для расчета вероятности состояния системы P_j
double calculate_Pj(int j, int m, double alpha, double P0)
{
    if (j <= m)
    {
        return (pow(m * alpha, j) / factorial(j)) * P0;
    }
    else
    {
        return (pow(m, m) * pow(alpha, j) / (factorial(j) * factorial(j - m))) * P0;
    }
}

// Функция для расчета коэффициента готовности S
double calculate_S(int N, int n, int m, double lambda, double mu)
{
    double alpha = mu / lambda;
    double sum = 0.0;

    // Расчет P0
    double P0 = 1.0;
    for (int l = 0; l <= N - m; l++)
    {
        sum += pow(m * alpha, l) / factorial(l);
    }
    for (int l = N - m + 1; l <= N; l++)
    {
        sum += (pow(m, m) * pow(alpha, l)) / (factorial(l) * factorial(l - m));
    }
    P0 = 1.0 / sum;

    // Расчет S
    double S = 0.0;
    for (int j = n; j <= N; j++)
    {
        S += calculate_Pj(j, m, alpha, P0);
    }
    return S;
}

// Функция для расчета функции оперативной надежности R*(t)
double calculate_R_star(double t, int N, int n, int m, double lambda, double mu)
{
    double alpha = mu / lambda;
    double sum = 0.0;

    // Расчет P0
    double P0 = 1.0;
    for (int l = 0; l <= N - m; l++)
    {
        sum += pow(m * alpha, l) / factorial(l);
    }
    for (int l = N - m + 1; l <= N; l++)
    {
        sum += (pow(m, m) * pow(alpha, l)) / (factorial(l) * factorial(l - m));
    }
    P0 = 1.0 / sum;

    // Расчет R*(t)
    double R_star = 0.0;
    for (int i = n; i <= N; i++)
    {
        double Q_i = 0.0;
        for (int l = 0; l <= i - n; l++)
        {
            Q_i += (pow(i * lambda * t, l) / factorial(l)) * exp(-i * lambda * t);
        }
        R_star += calculate_Pj(i, m, alpha, P0) * Q_i;
    }
    return R_star;
}

// Функция для расчета функции оперативной восстановимости U*(t)
double calculate_U_star(double t, int N, int n, int m, double lambda, double mu)
{
    double alpha = mu / lambda;
    double sum = 0.0;

    // Расчет P0
    double P0 = 1.0;
    for (int l = 0; l <= N - m; l++)
    {
        sum += pow(m * alpha, l) / factorial(l);
    }
    for (int l = N - m + 1; l <= N; l++)
    {
        sum += (pow(m, m) * pow(alpha, l)) / (factorial(l) * factorial(l - m));
    }
    P0 = 1.0 / sum;

    // Расчет U*(t)
    double U_star = 1.0;
    for (int i = 0; i < n; i++)
    {
        double sum_r = 0.0;
        for (int r = 0; r <= n - i - 1; r++)
        {
            sum_r += (pow(i * lambda * t, r) / factorial(r)) * exp(-i * lambda * t);
        }
        U_star -= calculate_Pj(i, m, alpha, P0) * sum_r;
    }
    return U_star;
}

void write_to_file(const char *filename, int n, double value)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "%d %.10f\n", n, value);
    fclose(file);
}

int main()
{
    // Параметры системы
    int N = 10;            // Общее количество ЭМ
    int n = 8;             // Количество ЭМ в основной подсистеме
    int m = 1;             // Количество восстанавливающих устройств
    double lambda = 0.024; // Интенсивность отказов
    double mu = 0.71;      // Интенсивность восстановления

    printf("N = %d\n", N);
    for (int n_ = n; n_ <= N; n_++)
    {
        char filename[50];
        sprintf(filename, "R_star_%d.txt", n_);
        printf("n = %d\n", n_);
        // Расчет функции оперативной надежности R*(t)
        for (int t = 0; t <= 24; t += 2)
        {
            double R_star = calculate_R_star(t, N, n_, m, lambda, mu);
            printf("R*(%d) = %.10f\n", t, R_star);
            write_to_file(filename, t, R_star);
        }
    }

    // Расчет функции оперативной восстановимости U*(t)
    n = 10;
    N = 16;

    printf("N = %d\n", N);
    for (int n_ = n; n_ <= N; n_++)
    {
        char filename[50];
        sprintf(filename, "U_star_%d.txt", n_);
        printf("n = %d\n", n_);
        for (int t = 0; t <= 24; t += 2)
        {
            double U_star = calculate_U_star(t, N, n_, m, lambda, mu);
            printf("U*(%d) = %.10f\n", t, U_star);
            write_to_file(filename, t, U_star);
        }
    }

    // Расчет коэффициента готовности S
    for (int n_ = n; n_ <= N; n_++)
    {
        char filename1[50];
        char filename2[50];
        sprintf(filename1, "S1_m1.txt");
        sprintf(filename2, "S1_m16.txt");
        printf("n = %d\n", n_);

        double S = calculate_S(N, n_, m, lambda, mu);
        write_to_file(filename1, n, S);
        printf("Коэффициент готовности S для m = 1: %f\n", S);
        S = calculate_S(N, n_, 16, lambda, mu);
        printf("Коэффициент готовности S для m = 16: %f\n", S);
        write_to_file(filename2, n, S);
    }

    return 0;
}