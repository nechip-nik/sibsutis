#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <random>
#include <algorithm>
#include <omp.h>

// Структура для хранения матрицы в формате CSR
struct CSR {
    size_t n;                   // Размерность матрицы (n x n)
    size_t nnz;                 // Количество ненулевых элементов
    std::vector<size_t> row_ptr; // Указатели на начало строк (размер n + 1)
    std::vector<int>    col_idx; // Индексы столбцов (размер nnz)
    std::vector<double> val;     // Значения (размер nnz)
};


void spmv_csr_naive(const CSR& A, const std::vector<double>& x, std::vector<double>& y) {
    std::fill(y.begin(), y.end(), 0.0);
    for (size_t i = 0; i < A.n; ++i) {
        double sum = 0.0;
        for (size_t j = A.row_ptr[i]; j < A.row_ptr[i + 1]; ++j) {
            sum += A.val[j] * x[A.col_idx[j]];
        }
        y[i] = sum;
    }
}

void spmv_csr_unroll(const CSR& A, const std::vector<double>& x, std::vector<double>& y) {
    std::fill(y.begin(), y.end(), 0.0);
    for (size_t i = 0; i < A.n; ++i) {
        size_t start = A.row_ptr[i];
        size_t end   = A.row_ptr[i + 1];
        
        double s0 = 0.0, s1 = 0.0, s2 = 0.0, s3 = 0.0;
        size_t j = start;
        
        for (; j + 3 < end; j += 4) {
            s0 += A.val[j]     * x[A.col_idx[j]];
            s1 += A.val[j + 1] * x[A.col_idx[j + 1]];
            s2 += A.val[j + 2] * x[A.col_idx[j + 2]];
            s3 += A.val[j + 3] * x[A.col_idx[j + 3]];
        }
        
        // Остаток
        double sum = s0 + s1 + s2 + s3;
        for (; j < end; ++j) {
            sum += A.val[j] * x[A.col_idx[j]];
        }
        y[i] = sum;
    }
}

void spmv_csr_omp(const CSR& A, const std::vector<double>& x, std::vector<double>& y) {
    std::fill(y.begin(), y.end(), 0.0);
    
    #pragma omp parallel for schedule(dynamic, 256)
    for (size_t i = 0; i < A.n; ++i) {
        size_t start = A.row_ptr[i];
        size_t end   = A.row_ptr[i + 1];
        
        double s0 = 0.0, s1 = 0.0, s2 = 0.0, s3 = 0.0;
        size_t j = start;
        
        // Префетчинг первых элементов строки
        for (size_t k = start; k < start + 16 && k < end; ++k) {
            __builtin_prefetch(&x[A.col_idx[k]], 0, 1);
        }

        for (; j + 3 < end; j += 4) {
            if (j + 16 < end) {
                __builtin_prefetch(&x[A.col_idx[j + 16]], 0, 1);
            }
            
            s0 += A.val[j]     * x[A.col_idx[j]];
            s1 += A.val[j + 1] * x[A.col_idx[j + 1]];
            s2 += A.val[j + 2] * x[A.col_idx[j + 2]];
            s3 += A.val[j + 3] * x[A.col_idx[j + 3]];
        }
        
        double sum = s0 + s1 + s2 + s3;
        for (; j < end; ++j) {
            sum += A.val[j] * x[A.col_idx[j]];
        }
        y[i] = sum;
    }
}

CSR generate_random_csr(size_t n, size_t nnz_per_row) {
    CSR A;
    A.n = n;
    A.nnz = n * nnz_per_row;
    A.row_ptr.resize(n + 1, 0);
    A.col_idx.resize(A.nnz);
    A.val.resize(A.nnz);

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> col_dist(0, n - 1);
    std::uniform_real_distribution<double> val_dist(1.0, 10.0);

    size_t idx = 0;
    for (size_t i = 0; i < n; ++i) {
        A.row_ptr[i] = idx;
        for (size_t k = 0; k < nnz_per_row; ++k) {
            A.col_idx[idx] = col_dist(rng);
            A.val[idx] = val_dist(rng);
            idx++;
        }
    }
    A.row_ptr[n] = idx;
    return A;
}

double check_error(const std::vector<double>& y1, const std::vector<double>& y2) {
    double max_err = 0.0;
    for (size_t i = 0; i < y1.size(); ++i) {
        max_err = std::max(max_err, std::abs(y1[i] - y2[i]));
    }
    return max_err;
}

int main(int argc, char** argv) {
    size_t n = 1000000;    
    size_t nnz_per_row = 64; 
    
    if (argc > 1) n = std::stoull(argv[1]);
    if (argc > 2) nnz_per_row = std::stoull(argv[2]);

    std::cout << "Generating CSR matrix " << n << "x" << n 
              << ", nnz per row: " << nnz_per_row << "...\n";
    CSR A = generate_random_csr(n, nnz_per_row);

    std::vector<double> x(n, 1.0);
    std::vector<double> y_naive(n, 0.0);
    std::vector<double> y_unroll(n, 0.0);
    std::vector<double> y_omp(n, 0.0);

    // 1. Наивная (эталон)
    auto t1 = std::chrono::high_resolution_clock::now();
    spmv_csr_naive(A, x, y_naive);
    auto t2 = std::chrono::high_resolution_clock::now();
    
    // 2. Unroll
    auto t3 = std::chrono::high_resolution_clock::now();
    spmv_csr_unroll(A, x, y_unroll);
    auto t4 = std::chrono::high_resolution_clock::now();
    
    // 3. OpenMP
    auto t5 = std::chrono::high_resolution_clock::now();
    spmv_csr_omp(A, x, y_omp);
    auto t6 = std::chrono::high_resolution_clock::now();

    // Проверка корректности
    double err1 = check_error(y_naive, y_unroll);
    double err2 = check_error(y_naive, y_omp);
    if (err1 > 1e-9 || err2 > 1e-9) {
        std::cerr << "ERROR: Incorrect results! err1=" << err1 << " err2=" << err2 << "\n";
    } else {
        std::cout << "Correctness: OK\n";
    }

    // Вывод результатов
    auto ms = [](auto start, auto end) {
        return std::chrono::duration<double, std::milli>(end - start).count();
    };

    double t_naive  = ms(t1, t2);
    double t_unroll = ms(t3, t4);
    double t_omp    = ms(t5, t6);

    std::cout << "\n--- Results ---\n";
    std::cout << "naive      " << t_naive  << " ms\n";
    std::cout << "unroll     " << t_unroll << " ms  (x" << t_naive/t_unroll << " vs naive)\n";
    std::cout << "omp        " << t_omp    << " ms  (x" << t_naive/t_omp    << " vs naive)\n";

    return 0;
}