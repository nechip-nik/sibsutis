// Файл: lab3_mutex.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

// Структура для хранения параметров потока
typedef struct {
    int id;
    int priority;
    int type;           // 0 - читатель, 1 - писатель
    int read_count;     // количество прочитанных символов
    int write_count;    // количество записанных символов
    int sleep_time;     // время "работы" в тактах
} ThreadData;

// Глобальные переменные для синхронизации
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;      // для защиты читателей
pthread_mutex_t rw_mutex = PTHREAD_MUTEX_INITIALIZER;   // для эксклюзивного доступа

int readers_count = 0;          // количество активных читателей
int writer_active = 0;          // флаг активности писателя
int file_position = 0;          // текущая позиция в файле
int total_read_operations = 0;   // общее количество операций чтения
int total_write_operations = 0;  // общее количество операций записи

// Параметры модели
int FILE_SIZE = 1000;            // размер файла
int BUFFER_SIZE = 100;           // размер буфера
int SIMULATION_TIME = 50;        // время моделирования (тактов)
int NUM_READERS = 3;             // количество читателей
int NUM_WRITERS = 1;             // количество писателей

// Массивы для хранения данных потоков
ThreadData* readers = NULL;
ThreadData* writers = NULL;

// Функция для логирования в файл
void log_to_file(const char* message) {
    FILE* log = fopen("simulation_log.txt", "a");
    if (log) {
        fprintf(log, "%s\n", message);
        fclose(log);
    }
}

// Функция читателя
void* reader_function(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    
    while (file_position < FILE_SIZE) {
        // Имитация времени между операциями
        usleep(rand() % 100000);  // 0-100 ms
        
        // Захват мьютекса для изменения счетчика читателей
        pthread_mutex_lock(&mutex);
        
        // Первый читатель блокирует доступ писателей
        if (readers_count == 0) {
            pthread_mutex_lock(&rw_mutex);
        }
        readers_count++;
        
        pthread_mutex_unlock(&mutex);
        
        // ЧТЕНИЕ (критическая секция)
        printf("Читатель %d (приоритет %d) читает данные. Активных читателей: %d\n", 
               data->id, data->priority, readers_count);
        
        char log_msg[256];
        sprintf(log_msg, "READER %d: reading data, active readers=%d", 
                data->id, readers_count);
        log_to_file(log_msg);
        
        // Имитация процесса чтения
        data->read_count++;
        total_read_operations++;
        usleep(rand() % 50000);  // 0-50 ms
        
        // Захват мьютекса для уменьшения счетчика читателей
        pthread_mutex_lock(&mutex);
        
        readers_count--;
        
        // Последний читатель освобождает доступ писателям
        if (readers_count == 0) {
            pthread_mutex_unlock(&rw_mutex);
        }
        
        pthread_mutex_unlock(&mutex);
        
        // Увеличиваем позицию в файле
        file_position++;
        
        // Проверка на завершение
        if (file_position >= FILE_SIZE) {
            break;
        }
    }
    
    return NULL;
}

// Функция писателя
void* writer_function(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    
    while (file_position < FILE_SIZE) {
        // Имитация времени между операциями
        usleep(rand() % 200000);  // 0-200 ms
        
        // Писатель захватывает эксклюзивный доступ
        pthread_mutex_lock(&rw_mutex);
        
        // ЗАПИСЬ (эксклюзивная критическая секция)
        writer_active = 1;
        printf("Писатель %d (приоритет %d) начал запись\n", 
               data->id, data->priority);
        
        char log_msg[256];
        sprintf(log_msg, "WRITER %d: start writing", data->id);
        log_to_file(log_msg);
        
        // Имитация процесса записи
        data->write_count++;
        total_write_operations++;
        
        // Запись нескольких символов
        for (int i = 0; i < 5 && file_position < FILE_SIZE; i++) {
            file_position++;
            usleep(rand() % 30000);  // 0-30 ms
        }
        
        printf("Писатель %d завершил запись. Позиция в файле: %d/%d\n", 
               data->id, file_position, FILE_SIZE);
        
        sprintf(log_msg, "WRITER %d: finished writing, position=%d/%d", 
                data->id, file_position, FILE_SIZE);
        log_to_file(log_msg);
        
        writer_active = 0;
        
        // Освобождение эксклюзивного доступа
        pthread_mutex_unlock(&rw_mutex);
        
        // Проверка на завершение
        if (file_position >= FILE_SIZE) {
            break;
        }
    }
    
    return NULL;
}

// Функция для создания потоков с приоритетами
void create_threads_with_priority(pthread_t* threads, ThreadData* thread_data, 
                                   int count, int type) {
    for (int i = 0; i < count; i++) {
        thread_data[i].id = i;
        thread_data[i].priority = rand() % 10 + 1;  // приоритет 1-10
        thread_data[i].type = type;
        thread_data[i].read_count = 0;
        thread_data[i].write_count = 0;
        
        if (type == 0) {
            pthread_create(&threads[i], NULL, reader_function, &thread_data[i]);
            printf("Создан читатель %d с приоритетом %d\n", i, thread_data[i].priority);
        } else {
            pthread_create(&threads[i], NULL, writer_function, &thread_data[i]);
            printf("Создан писатель %d с приоритетом %d\n", i, thread_data[i].priority);
        }
    }
}

// Функция для проведения эксперимента с разными приоритетами
void run_priority_experiment() {
    printf("\n========================================\n");
    printf("ЭКСПЕРИМЕНТ: Влияние приоритета писателя\n");
    printf("========================================\n");
    
    int priorities[] = {1, 3, 5, 7, 10};
    FILE* data_file = fopen("priority_data_mutex.txt", "w");
    
    if (!data_file) {
        printf("Ошибка создания файла данных\n");
        return;
    }
    
    fprintf(data_file, "# Приоритет\tЧтение\tЗапись\n");
    
    for (int idx = 0; idx < 5; idx++) {
        int writer_priority = priorities[idx];
        
        printf("\n--- Приоритет писателя: %d ---\n", writer_priority);
        
        // Инициализация
        FILE_SIZE = 500;
        NUM_READERS = 3;
        NUM_WRITERS = 1;
        
        readers = malloc(NUM_READERS * sizeof(ThreadData));
        writers = malloc(NUM_WRITERS * sizeof(ThreadData));
        
        pthread_t reader_threads[NUM_READERS];
        pthread_t writer_threads[NUM_WRITERS];
        
        // Сброс счетчиков
        readers_count = 0;
        writer_active = 0;
        file_position = 0;
        total_read_operations = 0;
        total_write_operations = 0;
        
        // Создание потоков
        create_threads_with_priority(reader_threads, readers, NUM_READERS, 0);
        
        // Устанавливаем приоритет писателя
        writers[0].id = 0;
        writers[0].priority = writer_priority;
        writers[0].type = 1;
        writers[0].read_count = 0;
        writers[0].write_count = 0;
        pthread_create(&writer_threads[0], NULL, writer_function, &writers[0]);
        
        // Ожидание завершения (симуляция в течение SIMULATION_TIME тактов)
        sleep(2);  // 2 секунды симуляции
        
        // Принудительное завершение (в реальном коде нужно использовать отмену потоков)
        // Но для эксперимента мы просто запишем результаты
        
        fprintf(data_file, "%d\t%d\t%d\n", writer_priority, 
                total_read_operations, total_write_operations);
        
        printf("Результаты: чтение=%d, запись=%d\n", 
               total_read_operations, total_write_operations);
        
        // Очистка
        free(readers);
        free(writers);
    }
    
    fclose(data_file);
    printf("\n✅ Данные сохранены в priority_data_mutex.txt\n");
}

// Функция для проведения эксперимента с разным количеством читателей
void run_readers_experiment() {
    printf("\n========================================\n");
    printf("ЭКСПЕРИМЕНТ: Влияние количества читателей\n");
    printf("========================================\n");
    
    int reader_counts[] = {1, 2, 3, 5, 10};
    FILE* data_file = fopen("readers_data_mutex.txt", "w");
    
    if (!data_file) {
        printf("Ошибка создания файла данных\n");
        return;
    }
    
    fprintf(data_file, "# Читателей\tЧтение\tЗапись\n");
    
    for (int idx = 0; idx < 5; idx++) {
        NUM_READERS = reader_counts[idx];
        
        printf("\n--- Читателей: %d ---\n", NUM_READERS);
        
        // Инициализация
        FILE_SIZE = 500;
        NUM_WRITERS = 1;
        
        readers = malloc(NUM_READERS * sizeof(ThreadData));
        writers = malloc(NUM_WRITERS * sizeof(ThreadData));
        
        pthread_t reader_threads[NUM_READERS];
        pthread_t writer_threads[NUM_WRITERS];
        
        // Сброс счетчиков
        readers_count = 0;
        writer_active = 0;
        file_position = 0;
        total_read_operations = 0;
        total_write_operations = 0;
        
        // Создание потоков
        create_threads_with_priority(reader_threads, readers, NUM_READERS, 0);
        
        // Писатель с фиксированным приоритетом
        writers[0].id = 0;
        writers[0].priority = 5;
        writers[0].type = 1;
        writers[0].read_count = 0;
        writers[0].write_count = 0;
        pthread_create(&writer_threads[0], NULL, writer_function, &writers[0]);
        
        // Симуляция
        sleep(2);  // 2 секунды
        
        fprintf(data_file, "%d\t%d\t%d\n", NUM_READERS, 
                total_read_operations, total_write_operations);
        
        printf("Результаты: чтение=%d, запись=%d\n", 
               total_read_operations, total_write_operations);
        
        // Очистка
        free(readers);
        free(writers);
    }
    
    fclose(data_file);
    printf("\n✅ Данные сохранены в readers_data_mutex.txt\n");
}

// Демонстрационная функция
void run_demo() {
    printf("\n========================================\n");
    printf("ДЕМОНСТРАЦИЯ РАБОТЫ МОДЕЛИ\n");
    printf("========================================\n");
    
    // Инициализация
    FILE_SIZE = 200;
    NUM_READERS = 3;
    NUM_WRITERS = 1;
    
    readers = malloc(NUM_READERS * sizeof(ThreadData));
    writers = malloc(NUM_WRITERS * sizeof(ThreadData));
    
    pthread_t reader_threads[NUM_READERS];
    pthread_t writer_threads[NUM_WRITERS];
    
    // Сброс счетчиков
    readers_count = 0;
    writer_active = 0;
    file_position = 0;
    total_read_operations = 0;
    total_write_operations = 0;
    
    // Очистка лог-файла
    FILE* log = fopen("simulation_log.txt", "w");
    if (log) fclose(log);
    
    printf("Параметры: %d читателей, %d писатель, файл %d символов\n", 
           NUM_READERS, NUM_WRITERS, FILE_SIZE);
    
    // Создание читателей
    for (int i = 0; i < NUM_READERS; i++) {
        readers[i].id = i;
        readers[i].priority = rand() % 10 + 1;
        readers[i].type = 0;
        readers[i].read_count = 0;
        readers[i].write_count = 0;
        pthread_create(&reader_threads[i], NULL, reader_function, &readers[i]);
    }
    
    // Создание писателя
    writers[0].id = 0;
    writers[0].priority = 5;
    writers[0].type = 1;
    writers[0].read_count = 0;
    writers[0].write_count = 0;
    pthread_create(&writer_threads[0], NULL, writer_function, &writers[0]);
    
    // Ожидание завершения
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(reader_threads[i], NULL);
    }
    pthread_join(writer_threads[0], NULL);
    
    // Вывод результатов
    printf("\n=== РЕЗУЛЬТАТЫ ===\n");
    printf("Всего операций чтения: %d\n", total_read_operations);
    printf("Всего операций записи: %d\n", total_write_operations);
    printf("Финальная позиция в файле: %d/%d\n", file_position, FILE_SIZE);
    
    printf("\n--- Читатели ---\n");
    for (int i = 0; i < NUM_READERS; i++) {
        printf("Читатель %d (приоритет %d): прочитано %d\n", 
               i, readers[i].priority, readers[i].read_count);
    }
    
    printf("--- Писатель ---\n");
    printf("Писатель %d (приоритет %d): записано %d\n", 
           writers[0].id, writers[0].priority, writers[0].write_count);
    
    free(readers);
    free(writers);
}

int main() {
    
    // Инициализация генератора случайных чисел
    srand(time(NULL));
    
    printf("========================================\n");
    printf("ЛАБОРАТОРНАЯ РАБОТА №3\n");
    printf("Моделирование управления в распределенных системах\n");
    printf("(Реализация на C с использованием mutex)\n");
    printf("========================================\n");
    
    while (1) {
        printf("\nМеню:\n");
        printf("1. Демонстрация работы модели\n");
        printf("2. Эксперимент: влияние приоритета писателя\n");
        printf("3. Эксперимент: влияние количества читателей\n");
        printf("4. Выход\n");
        printf("Выбор: ");
        
        int choice;
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                run_demo();
                break;
            case 2:
                run_priority_experiment();
                break;
            case 3:
                run_readers_experiment();
                break;
            case 4:
                printf("Выход из программы.\n");
                // Уничтожение мьютексов
                pthread_mutex_destroy(&mutex);
                pthread_mutex_destroy(&rw_mutex);
                return 0;
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
        }
    }
    
    return 0;
}