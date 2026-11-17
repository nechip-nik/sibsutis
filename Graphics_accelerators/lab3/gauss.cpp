#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <omp.h>

using namespace std;
using namespace std::chrono;

// Генерация матрицы в flat-формате
void generate_system(vector<double>& A, vector<double>& b, int n) {
    mt19937 rng(42);
    uniform_real_distribution<double> dist(-10.0, 10.0);
    A.assign(n * n, 0.0);
    b.assign(n, 0.0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i * n + j] = dist(rng);
        }
        b[i] = dist(rng);
    }
}

// Метод Гаусса с OpenMP (flat array)
vector<double> gauss_omp(vector<double> A, vector<double> b, int n) {
    vector<double> x(n);

    for (int k = 0; k < n; ++k) {
        // Поиск ведущего элемента (последовательно)
        int pivot = k;
        double maxv = fabs(A[k * n + k]);
        for (int i = k + 1; i < n; ++i) {
            if (fabs(A[i * n + k]) > maxv) {
                maxv = fabs(A[i * n + k]);
                pivot = i;
            }
        }

        if (maxv < 1e-12) {
            cerr << "Матрица вырождена!\n";
            return x;
        }

        // Обмен строк
        if (pivot != k) {
            for (int j = 0; j < n; ++j) {
                swap(A[k * n + j], A[pivot * n + j]);
            }
            swap(b[k], b[pivot]);
        }

        double pivot_val = A[k * n + k];

        // Параллельная обработка строк
        #pragma omp parallel for schedule(static)
        for (int i = k + 1; i < n; ++i) {
            double factor = A[i * n + k] / pivot_val;
            A[i * n + k] = 0.0;
            for (int j = k + 1; j < n; ++j) {
                A[i * n + j] -= factor * A[k * n + j];
            }
            b[i] -= factor * b[k];
        }
    }

    // Обратный ход
    for (int i = n - 1; i >= 0; --i) {
        double s = b[i];
        for (int j = i + 1; j < n; ++j) {
            s -= A[i * n + j] * x[j];
        }
        x[i] = s / A[i * n + i];
    }
    return x;
}

int main(int argc, char** argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 500;
    cout << "n = " << n << ", потоков: " << omp_get_max_threads() << "\n";

    vector<double> A, b;
    generate_system(A, b, n);

    auto start = high_resolution_clock::now();
    vector<double> x = gauss_omp(A, b, n);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Время: " << fixed << setprecision(3) << duration.count() / 1000.0 << " с\n";
    return 0;
}