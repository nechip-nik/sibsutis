#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000  // Размер матрицы

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double A[N][N], b[N], x[N], x_new[N];
    int i, j, k, iter;
    double sum, error, max_error;

    // Инициализация матрицы A и вектора b
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = 1.0;  // Заполните матрицу A
        }
        b[i] = i;  // Заполните вектор b
        x[i] = 0.0;  // Начальное приближение
    }
    double start_time = MPI_Wtime();
    // Параллельный цикл Якоби
    for (iter = 0; iter < 1000; iter++) {
        
        for (i = rank; i < N; i += size) {
            sum = 0.0;
            for (j = 0; j < N; j++) {
                if (j != i) {
                    sum += A[i][j] * x[j];
                }
            }
            x_new[i] = (b[i] - sum) / A[i][i];
        }

        // Обмен данными между процессами
        MPI_Allgather(MPI_IN_PLACE, 0, MPI_DOUBLE, x_new, 1, MPI_DOUBLE, MPI_COMM_WORLD);

        // Вычисление ошибки
        error = 0.0;
        for (i = rank; i < N; i += size) {
            error += (x_new[i] - x[i]) * (x_new[i] - x[i]);
        }
        // MPI_Reduce(&error, &max_error, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

        // Обновление вектора x
        for (i = rank; i < N; i += size) {
            x[i] = x_new[i];
        }

        // if (rank == 0) {
        //     printf("Iteration %d: max error = %f\n", iter, max_error);
        // }
    }
    double end_time = MPI_Wtime();
    double execution_time = end_time - start_time;

    if (rank == 0) {
        printf("Время выполнения: %f секунд\n", execution_time);
    }

    MPI_Finalize();
    return 0;
}