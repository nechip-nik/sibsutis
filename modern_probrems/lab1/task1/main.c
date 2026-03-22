#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct {
    double k;  
    int rows;  
    int cols;  
    int** data; 
} Matrix;

typedef struct {
    double k;  
    int size;  
    int* data; 
} Vector;

Matrix* create_matrix(double k, int rows, int cols) {
    Matrix* mat = (Matrix*)malloc(sizeof(Matrix));
    mat->k = k;
    mat->rows = rows;
    mat->cols = cols;
    mat->data = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        mat->data[i] = (int*)malloc(cols * sizeof(int));
    }
    return mat;
}

Vector* create_vector(double k, int size) {
    Vector* vec = (Vector*)malloc(sizeof(Vector));
    vec->k = k;
    vec->size = size;
    vec->data = (int*)malloc(size * sizeof(int));
    return vec;
}

void free_matrix(Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        free(mat->data[i]);
    }
    free(mat->data);
    free(mat);
}

void free_vector(Vector* vec) {
    free(vec->data);
    free(vec);
}


Matrix* matrix_multiply(Matrix* A, Matrix* B) {
    if (A->cols != B->rows) {
        printf("Error: Incompatible matrix dimensions for multiplication\n");
        return NULL;
    }
    
    Matrix* C = create_matrix(A->k * B->k, A->rows, B->cols);
    
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->cols; j++) {
            int sum = 0;
            for (int k = 0; k < A->cols; k++) {
                sum += A->data[i][k] * B->data[k][j];
            }
            C->data[i][j] = sum;
        }
    }
    
    return C;
}

Vector* matrix_vector_multiply(Matrix* A, Vector* B) {
    if (A->cols != B->size) {
        printf("Error: Incompatible dimensions for matrix-vector multiplication\n");
        return NULL;
    }
    
    Vector* C = create_vector(A->k * B->k, A->rows);
    
    for (int i = 0; i < A->rows; i++) {
        int sum = 0;
        for (int j = 0; j < A->cols; j++) {
            sum += A->data[i][j] * B->data[j];
        }
        C->data[i] = sum;
    }
    
    return C;
}

Matrix* tensor_matrix_multiply(Matrix* A, Matrix* B) {
    Matrix* C = create_matrix(A->k * B->k, A->rows * B->rows, A->cols * B->cols);
    
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->cols; j++) {
            for (int k = 0; k < B->rows; k++) {
                for (int l = 0; l < B->cols; l++) {
                    int row_idx = i * B->rows + k;
                    int col_idx = j * B->cols + l;
                    C->data[row_idx][col_idx] = A->data[i][j] * B->data[k][l];
                }
            }
        }
    }
    
    return C;
}

// Тензорное произведение векторов
Vector* tensor_vector_multiply(Vector* A, Vector* B) {
    Vector* C = create_vector(A->k * B->k, A->size * B->size);
    
    for (int i = 0; i < A->size; i++) {
        for (int j = 0; j < B->size; j++) {
            int idx = i * B->size + j;
            C->data[idx] = A->data[i] * B->data[j];
        }
    }
    
    return C;
}

char* vector_to_bracket_notation(Vector* vec) {
    char* result = (char*)malloc(5000 * sizeof(char)); 
    strcpy(result, "");
    
    char temp[200]; 
    int first = 1;
    
    // Определяем количество кубитов по размеру вектора
    int num_qubits = (int)(log2(vec->size) + 0.5);
    
    for (int i = 0; i < vec->size; i++) {
        if (vec->data[i] != 0) {
            if (!first) {
                strcat(result, " + ");
            }
            
            double coeff = vec->k * vec->data[i];
            
            // Определение базисного вектора
            char basis[100] = "";
            int state = i;
            
            // Записываем биты в правильном порядке (старший бит первый)
            for (int j = num_qubits - 1; j >= 0; j--) {
                // Безопасное добавление символов
                char bit_char = (state & (1 << j)) ? '1' : '0';
                int len = strlen(basis);
                if (len < 98) { // Оставляем место для завершающего нуля
                    basis[len] = bit_char;
                    basis[len + 1] = '\0';
                }
            }
            
            // Форматируем коэффициент с проверкой длины
            if (fabs(fabs(coeff) - 1.0) < 1e-10) {
                if (coeff < 0) {
                    snprintf(temp, sizeof(temp), "-|%s⟩", basis);
                } else {
                    snprintf(temp, sizeof(temp), "|%s⟩", basis);
                }
            } else {
                // Округляем до 2 знаков после запятой для красоты
                double rounded_coeff = round(coeff * 100) / 100;
                if (fabs(rounded_coeff) < 1e-10) {
                    continue; // пропускаем нулевые коэффициенты
                }
                snprintf(temp, sizeof(temp), "%.2f|%s⟩", rounded_coeff, basis);
            }
            
            // Проверяем, не превысим ли мы размер результата
            if (strlen(result) + strlen(temp) < 4990) {
                strcat(result, temp);
            } else {
                strcat(result, "..."); // обрезаем если слишком длинный
                break;
            }
            first = 0;
        }
    }
    
    // Если все коэффициенты нулевые
    if (strlen(result) == 0) {
        strcpy(result, "0");
    }
    
    return result;
}

Vector* bracket_to_vector_notation(const char* bracket_str, int size) {
    Vector* vec = create_vector(1.0, size);
    
    for (int i = 0; i < size; i++) {
        vec->data[i] = 0;
    }
    char* copy = strdup(bracket_str);
    char* token = strtok(copy, "+");
    
    double first_coefficient = 1.0;
    int first_coeff_found = 0;
    
    while (token != NULL) {
        // Пропускаем пробелы
        char* cleaned_token = token;
        while (*cleaned_token == ' ') cleaned_token++;
        
        double coefficient = 1.0;
        char basis[100] = "";
        int parsed = 0;
        
        // Парсим коэффициент и базисный вектор
        if (sscanf(cleaned_token, "%lf|%[01]⟩", &coefficient, basis) == 2) {
            parsed = 1;
        } else if (sscanf(cleaned_token, "-|%[01]⟩", basis) == 1) {
            coefficient = -1.0;
            parsed = 1;
        } else if (sscanf(cleaned_token, "|%[01]⟩", basis) == 1) {
            coefficient = 1.0;
            parsed = 1;
        }
        
        if (parsed) {
            // Сохраняем первый коэффициент для общего k
            if (!first_coeff_found) {
                first_coefficient = fabs(coefficient);
                first_coeff_found = 1;
            }
            
            // Преобразуем бинарную строку в индекс
            int index = 0;
            int basis_len = strlen(basis);
            for (int i = 0; i < basis_len; i++) {
                if (basis[i] == '1') {
                    index |= (1 << (basis_len - 1 - i));
                }
            }
            
            // Записываем в вектор
            if (index < size) {
                // Нормализуем данные: делим на коэффициент и округляем до {-1, 0, 1}
                double normalized_value = coefficient / first_coefficient;
                if (fabs(normalized_value - 1.0) < 1e-10) {
                    vec->data[index] = 1;
                } else if (fabs(normalized_value + 1.0) < 1e-10) {
                    vec->data[index] = -1;
                } else {
                    vec->data[index] = 0;
                }
            }
        }
        
        token = strtok(NULL, "+");
    }
    
    free(copy);
    
    // Устанавливаем общий коэффициент
    if (first_coeff_found) {
        vec->k = first_coefficient;
    } else {
        vec->k = 1.0;
    }
    
    return vec;
}

void print_matrix(Matrix* mat) {
    printf("Matrix (k=%.2f):\n", mat->k);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%2d ", mat->data[i][j]);
        }
        printf("\n");
    }
}


void print_vector(Vector* vec) {
    printf("Vector (k=%.2f): [", vec->k);
    for (int i = 0; i < vec->size; i++) {
        printf("%2d", vec->data[i]);
        if (i < vec->size - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    Matrix* A = create_matrix(1.0, 2, 2);
    A->data[0][0] = 1; A->data[0][1] = 1;
    A->data[1][0] = 1; A->data[1][1] = -1;
    
    Matrix* B = create_matrix(0.5, 2, 2);
    B->data[0][0] = 1; B->data[0][1] = 0;
    B->data[1][0] = 0; B->data[1][1] = 1;
    
    Vector* v1 = create_vector(1.0/sqrt(2), 2);
    v1->data[0] = 1; v1->data[1] = 1;
    
    Vector* v2 = create_vector(1.0, 2);
    v2->data[0] = 1; v2->data[1] = 0;
    
    printf("=== Matrix Operations ===\n");
    
    // Умножение матриц
    Matrix* C = matrix_multiply(A, B);
    if (C) {
        printf("Matrix multiplication:\n");
        print_matrix(C);
        free_matrix(C);
    }
    
    // Умножение матрицы на вектор
    Vector* v3 = matrix_vector_multiply(A, v1);
    if (v3) {
        printf("Matrix-vector multiplication:\n");
        print_vector(v3);
        free_vector(v3);
    }
    
    // Тензорное произведение матриц
    Matrix* D = tensor_matrix_multiply(A, B);
    if (D) {
        printf("Tensor product of matrices:\n");
        print_matrix(D);
        free_matrix(D);
    }
    
    // Тензорное произведение векторов
Vector* v4 = tensor_vector_multiply(v1, v2);
if (v4) {
    printf("Tensor product of vectors:\n");
    print_vector(v4);
    
    // Перевод в скобочную нотацию
    char* bracket = vector_to_bracket_notation(v4);
    printf("Bracket notation: %s\n", bracket);
    
    // ОБРАТНЫЙ ПЕРЕВОД из скобочной нотации в вектор
    printf("\n=== Testing reverse conversion ===\n");
    Vector* v5 = bracket_to_vector_notation(bracket, 4);
    printf("Original bracket: %s\n", bracket);
    printf("Converted back to vector:\n");
    print_vector(v5);
    
    // Проверяем, что получили тот же результат
    char* bracket_back = vector_to_bracket_notation(v5);
    printf("Back to bracket: %s\n", bracket_back);
    
    // Сравниваем исходный и преобразованный векторы
    printf("Conversion successful: %s\n", 
           (v4->data[0] == v5->data[0] && v4->data[1] == v5->data[1] && 
            v4->data[2] == v5->data[2] && v4->data[3] == v5->data[3]) ? "YES" : "NO");
    
    free(bracket);
    free(bracket_back);
    free_vector(v4);
    free_vector(v5);
           }
}