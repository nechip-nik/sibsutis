#include <stdio.h>
#include <math.h>

double get_mu_l(double mu, int m, int l, int N)
{
    if ((N - m) <= l && l <= N)
    {
        return ((N - l) * mu);
    }
    else if (0 <= l && l < (N - m))
    {
        return (m * mu);
    }
    else
    {
        printf("Error!\n");
        return -1;
    }
}

double Theta(int n, double lambda, int m, double mu, int N)
{
    double theta = 0;

    theta += 1 / (n * lambda);

    for (int j = n + 1; j <= N; j++)
    {
        double first = 1 / (j * lambda);

        double second = 1;
        for (int l = n; l < j; l++)
        {
            double mu_l = 0;
            mu_l = get_mu_l(mu, m, l, N);

            second *= mu_l / (l * lambda);
        }

        theta += first * second;
    }

    return theta;
}

double T(int n, double lambda, int m, double mu, int N)
{
    if (n == 1)
    {
        return (1 / mu);
    }
    else if (n < 0)
    {
        printf("ERROR!\n");

        return -1;
    }

    double T = 0;
    double first = 1;

    for (int l = 1; l <= n - 1; l++)
    {
        double mu_l = get_mu_l(mu, m, l, N);

        first *= (1 / mu) * ((l * lambda) / mu_l);
    }

    double second = 0;
    for (int j = 1; j <= n - 1; j++)
    {
        double product = 1;
        for (int l = j; l <= n - 1; l++)
        {
            double mu_l = 0;
            mu_l = get_mu_l(mu, m, l, N);

            product *= (l * lambda) / mu_l;
        }
        second += (1 / (j * lambda)) * product;
    }
    T += (first + second);

    return T;
}

void write_to_file(const char *filename, int n, double value)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "%d %f\n", n, value);
    fclose(file);
}

int main()
{
    double lambda = 1e-5;
    int m = 1, mu = 1;
    int N1 = 65536, N2 = 1000;

#ifdef MU
    for (int mu_ = mu; mu_ <= 1000; mu_ *= 10)
    {
        char filename[50];
        sprintf(filename, "theta_mu_%d.txt", mu_);
        printf("mu = %d\n", mu_);
        for (int n = 65526; n <= N1; n++)
        {
            double theta = Theta(n, lambda, m, mu_, N1);
            printf("Mean time between failures, Theta = %f\n", theta);
            write_to_file(filename, n, theta);
        }
        putchar('\n');
    }

    int mu_values[] = {1, 2, 4, 6};
    lambda = 1e-3;

    for (int i = 0; i < 4; i++)
    {
        char filename[50];
        sprintf(filename, "T_mu_%d.txt", mu_values[i]);
        printf("mu = %d\n", mu_values[i]);
        for (int n = 900; n <= N2; n += 10)
        {
            double t = T(n, lambda, m, mu_values[i], N2);
            printf("Mean time to recovery, T = %f\n", t);
            write_to_file(filename, n, t);
        }
        putchar('\n');
    }
#endif

#ifdef LAMBDA
    for (double lambda_ = lambda; lambda_ >= 1e-9; lambda_ *= 1e-1)
    {
        char filename[50];
        sprintf(filename, "theta_lambda_%.9f.txt", lambda_);
        printf("lambda = %.9f\n", lambda_);
        for (int n = 65526; n <= N1; n++)
        {
            double theta = Theta(n, lambda_, m, mu, N1);
            printf("Mean time between failures, Theta = %f\n", theta);
            write_to_file(filename, n, theta);
        }
        putchar('\n');
    }

    N2 = 8192;
    for (double lambda_ = lambda; lambda_ >= 1e-9; lambda_ *= 1e-1)
    {
        char filename[50];
        sprintf(filename, "T_lambda_%.9f.txt", lambda_);
        printf("lambda = %.9f\n", lambda_);
        for (int n = 8092; n <= N2; n += 10)
        {
            double t = T(n, lambda_, m, mu, N2);
            printf("Mean time between recovery, T = %f\n", t);
            write_to_file(filename, n, t);
        }
        putchar('\n');
    }
#endif

#ifdef M
    for (int m_ = m; m_ <= 4; m_++)
    {
        char filename[50];
        sprintf(filename, "theta_m_%d.txt", m_);
        printf("m = %d\n", m_);
        for (int n = 65526; n <= N1; n++)
        {
            double theta = Theta(n, lambda, m_, mu, N1);
            printf("Mean time between failures, Theta = %f\n", theta);
            write_to_file(filename, n, theta);
        }
        putchar('\n');
    }

    N2 = 8192;
    for (int m_ = 1; m_ <= 4; m_++)
    {
        char filename[50];
        sprintf(filename, "T_m_%d.txt", m_);
        printf("m = %d\n", m_);
        for (int n = 8092; n <= N2; n += 10)
        {
            double t = T(n, lambda, m_, mu, N2);
            printf("Mean time between recovery, T = %f\n", t);
            write_to_file(filename, n, t);
        }
        putchar('\n');
    }
#endif
    return 0;
}