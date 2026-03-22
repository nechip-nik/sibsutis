#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <fstream>
#include <algorithm>

using namespace std;

// Класс процесса
class Process {
public:
    int id;
    int priority;
    int type; // 0 - читатель, 1 - писатель
    int buffer_pos; // текущая позиция в буфере
    int read_count; // количество прочитанных символов
    int write_count; // количество записанных символов
    
    Process(int _id, int _priority, int _type) {
        id = _id;
        priority = _priority;
        type = _type;
        buffer_pos = 0;
        read_count = 0;
        write_count = 0;
    }
};

class ReaderWriterModel {
private:
    vector<Process> readers;
    vector<Process> writers;
    vector<Process> all_processes;
    
    int file_position; // текущая позиция в файле
    int file_size; // размер файла
    int buffer_size; // размер буфера
    
    int active_readers; // количество активных читателей
    bool writer_active; // флаг активности писателя
    
    int total_cycles; // общее количество тактов
    
    random_device rd;
    mt19937 gen;
    uniform_real_distribution<> prob_dist;
    
    // Данные для анализа
    vector<int> reader_progress;
    vector<int> writer_progress;
    
public:
    ReaderWriterModel(int n_readers, int n_writers, int _file_size = 1000, int _buffer_size = 100) 
        : gen(rd()), prob_dist(0.0, 1.0) {
        
        file_size = _file_size;
        buffer_size = _buffer_size;
        file_position = 0;
        active_readers = 0;
        writer_active = false;
        total_cycles = 0;
        
        // Создание читателей
        for (int i = 0; i < n_readers; i++) {
            int priority = rand() % 10 + 1; // приоритет от 1 до 10
            readers.push_back(Process(i, priority, 0));
            all_processes.push_back(Process(i, priority, 0));
        }
        
        // Создание писателей
        for (int i = 0; i < n_writers; i++) {
            int priority = rand() % 10 + 1;
            writers.push_back(Process(n_readers + i, priority, 1));
            all_processes.push_back(Process(n_readers + i, priority, 1));
        }
        
        // Инициализация массивов для сбора данных
        reader_progress.resize(n_readers, 0);
        writer_progress.resize(n_writers, 0);
    }
    
    // Функция для выбора следующего процесса на основе приоритетов
    int selectNextProcess() {
        // Вычисляем сумму приоритетов
        int total_priority = 0;
        for (auto& p : all_processes) {
            total_priority += p.priority;
        }
        
        // Выбираем случайное число от 0 до total_priority
        uniform_int_distribution<> priority_dist(0, total_priority - 1);
        int random_value = priority_dist(gen);
        
        // Находим процесс, соответствующий случайному числу
        int cumulative = 0;
        for (size_t i = 0; i < all_processes.size(); i++) {
            cumulative += all_processes[i].priority;
            if (random_value < cumulative) {
                return i;
            }
        }
        return 0;
    }
    
    // Моделирование одного такта работы системы
    void simulateStep() {
        total_cycles++;
        
        // Если писатель активен, он продолжает писать
        if (writer_active) {
            // Находим активного писателя
            for (auto& writer : writers) {
                if (writer.write_count < file_size / 2) { // писатель ещё не закончил
                    // Записываем один символ
                    writer.write_count++;
                    file_position++;
                    
                    cout << "Писатель " << writer.id << " пишет символ " 
                         << writer.write_count << "/" << file_size/2 << endl;
                    
                    // Проверяем, закончил ли писатель
                    if (writer.write_count >= file_size / 2) {
                        writer_active = false;
                        cout << "Писатель " << writer.id << " завершил запись\n";
                    }
                    return;
                }
            }
        }
        
        // Если нет активного писателя, выбираем следующий процесс
        int next_process_idx = selectNextProcess();
        Process& selected = all_processes[next_process_idx];
        
        if (selected.type == 0) { // Читатель
            // Проверяем, может ли читатель работать
            if (!writer_active) {
                active_readers++;
                
                // Читаем символ из файла
                if (selected.buffer_pos < buffer_size && file_position > 0) {
                    selected.read_count++;
                    selected.buffer_pos++;
                    
                    cout << "Читатель " << selected.id << " читает символ " 
                         << selected.read_count << " (активных читателей: " 
                         << active_readers << ")" << endl;
                    
                    // Если буфер заполнен, сбрасываем его
                    if (selected.buffer_pos >= buffer_size) {
                        selected.buffer_pos = 0;
                    }
                }
                
                active_readers--;
            } else {
                cout << "Читатель " << selected.id << " ждёт (активен писатель)\n";
            }
        } else { // Писатель
            // Писатель может начать работу, только если нет активных читателей
            if (active_readers == 0 && !writer_active) {
                writer_active = true;
                selected.write_count++;
                file_position++;
                
                cout << "Писатель " << selected.id << " начал запись (символ 1/" 
                     << file_size/2 << ")" << endl;
            } else {
                cout << "Писатель " << selected.id << " ждёт (активных читателей: " 
                     << active_readers << ")\n";
            }
        }
    }
    
    // Запуск моделирования на заданное количество тактов
    void runSimulation(int cycles, int writer_priority = 5) {
        cout << "\n=== Запуск моделирования ===\n";
        cout << "Приоритет писателя: " << writer_priority << "\n\n";
        
        // Устанавливаем приоритет писателя
        for (auto& writer : writers) {
            writer.priority = writer_priority;
        }
        
        // Обновляем приоритеты в общем списке
        for (auto& p : all_processes) {
            if (p.type == 1) {
                p.priority = writer_priority;
            }
        }
        
        // Моделирование
        for (int cycle = 0; cycle < cycles; cycle++) {
            cout << "\nТакт " << cycle + 1 << ":\n";
            simulateStep();
            
            // Сохраняем прогресс
            for (size_t i = 0; i < readers.size(); i++) {
                if (i < reader_progress.size()) {
                    reader_progress[i] = readers[i].read_count;
                }
            }
            for (size_t i = 0; i < writers.size(); i++) {
                if (i < writer_progress.size()) {
                    writer_progress[i] = writers[i].write_count;
                }
            }
        }
        
        // Вывод результатов
        printResults();
    }
    
    // Вывод результатов моделирования
    void printResults() {
        cout << "\n=== РЕЗУЛЬТАТЫ МОДЕЛИРОВАНИЯ ===\n";
        cout << "Всего тактов: " << total_cycles << endl;
        cout << "Файл записан на " << file_position << " символов из " << file_size << endl;
        
        cout << "\n--- Читатели ---\n";
        for (auto& reader : readers) {
            cout << "Читатель " << reader.id << " (приоритет " << reader.priority 
                 << "): прочитано " << reader.read_count << " символов\n";
        }
        
        cout << "\n--- Писатели ---\n";
        for (auto& writer : writers) {
            cout << "Писатель " << writer.id << " (приоритет " << writer.priority 
                 << "): записано " << writer.write_count << " символов\n";
        }
    }
    
    // Получение данных для построения графиков
    vector<int> getReaderProgress() {
        return reader_progress;
    }
    
    vector<int> getWriterProgress() {
        return writer_progress;
    }
};

// Класс для проведения экспериментов
class Experiment {
public:
    static void runPriorityExperiment() {
        cout << "\n========================================\n";
        cout << "ЭКСПЕРИМЕНТ: Влияние приоритета писателя\n";
        cout << "========================================\n";
        
        vector<int> priorities = {1, 3, 5, 7, 10};
        vector<vector<int>> results_readers;
        vector<vector<int>> results_writers;
        
        ofstream dataFile("priority_data.txt");
        dataFile << "# Приоритет писателя\tПрочитано символов\tЗаписано символов\n";
        
        for (int priority : priorities) {
            cout << "\n--- Приоритет писателя: " << priority << " ---\n";
            
            ReaderWriterModel model(3, 1, 500, 50);
            model.runSimulation(100, priority);
            
            int total_read = 0;
            int total_write = 0;
            
            auto reader_prog = model.getReaderProgress();
            auto writer_prog = model.getWriterProgress();
            
            for (int r : reader_prog) total_read += r;
            for (int w : writer_prog) total_write += w;
            
            dataFile << priority << "\t" << total_read << "\t" << total_write << "\n";
        }
        
        dataFile.close();
        cout << "\n✅ Данные сохранены в priority_data.txt\n";
    }
    
    static void runReadersCountExperiment() {
        cout << "\n========================================\n";
        cout << "ЭКСПЕРИМЕНТ: Влияние количества читателей\n";
        cout << "========================================\n";
        
        vector<int> reader_counts = {1, 2, 3, 5, 10};
        
        ofstream dataFile("readers_data.txt");
        dataFile << "# Количество читателей\tПрочитано символов\tЗаписано символов\n";
        
        for (int n_readers : reader_counts) {
            cout << "\n--- Читателей: " << n_readers << " ---\n";
            
            ReaderWriterModel model(n_readers, 1, 500, 50);
            model.runSimulation(100, 5);
            
            int total_read = 0;
            int total_write = 0;
            
            auto reader_prog = model.getReaderProgress();
            auto writer_prog = model.getWriterProgress();
            
            for (int r : reader_prog) total_read += r;
            for (int w : writer_prog) total_write += w;
            
            dataFile << n_readers << "\t" << total_read << "\t" << total_write << "\n";
        }
        
        dataFile.close();
        cout << "\n✅ Данные сохранены в readers_data.txt\n";
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    
    cout << "========================================\n";
    cout << "ЛАБОРАТОРНАЯ РАБОТА №3\n";
    cout << "Моделирование управления в распределенных системах\n";
    cout << "========================================\n";
    
    while (true) {
        cout << "\nВыберите режим работы:\n";
        cout << "1. Демонстрация работы модели\n";
        cout << "2. Эксперимент: влияние приоритета писателя\n";
        cout << "3. Эксперимент: влияние количества читателей\n";
        cout << "4. Выход\n";
        cout << "Ваш выбор: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: {
                cout << "\nДЕМОНСТРАЦИЯ РАБОТЫ МОДЕЛИ\n";
                cout << "Параметры: 3 читателя, 1 писатель, файл 200 символов\n";
                
                ReaderWriterModel demo(3, 1, 200, 30);
                demo.runSimulation(50, 5);
                break;
            }
            
            case 2: {
                Experiment::runPriorityExperiment();
                break;
            }
            
            case 3: {
                Experiment::runReadersCountExperiment();
                break;
            }
            
            case 4: {
                cout << "Выход из программы.\n";
                return 0;
            }
            
            default:
                cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }
    
    return 0;
}