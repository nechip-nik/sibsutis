#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>
#include <algorithm>

// Структура для хранения информации о процессе
struct Process {
    int id;
    char type;           // 'R' - чтение, 'W' - запись
    int priority;        // приоритет (1 - низкий, больше - выше)
    int charsRead;       // сколько символов прочитано (для читателей)
    bool active;         // активен ли процесс в данный момент
};

class FileAccessModel {
private:
    std::vector<Process> processes;
    int currentWriterId;          // id пишущего процесса (-1 если нет)
    int activeReaders;            // количество активных читателей
    bool fileBusyByWriter;        // занят ли файл записью
    std::mt19937 rng;             // генератор случайных чисел
    int totalCycles;              // общее количество тактов
    int currentCycle;             // текущий такт

    // Выбор следующего активного процесса на основе приоритетов
    int selectNextProcess() {
        // Суммируем приоритеты всех процессов
        int totalPriority = 0;
        for (const auto& p : processes) {
            totalPriority += p.priority;
        }

        if (totalPriority == 0) return -1;

        // Случайный выбор с весами = приоритет
        std::uniform_int_distribution<int> dist(1, totalPriority);
        int roll = dist(rng);
        
        int cumulative = 0;
        for (size_t i = 0; i < processes.size(); ++i) {
            cumulative += processes[i].priority;
            if (roll <= cumulative) {
                return i;
            }
        }
        return -1;
    }

public:
    FileAccessModel(const std::vector<Process>& procs, int cycles, unsigned int seed = 42)
        : processes(procs), currentWriterId(-1), activeReaders(0), 
          fileBusyByWriter(false), rng(seed), totalCycles(cycles), currentCycle(0) {}

    // Запуск моделирования
    void run() {
        std::cout << "=== Моделирование доступа к файлу ===\n";
        std::cout << "Циклов: " << totalCycles << "\n";
        std::cout << "Процессы:\n";
        for (const auto& p : processes) {
            std::cout << "  ID=" << p.id << " Type=" << p.type 
                      << " Priority=" << p.priority << "\n";
        }
        std::cout << "------------------------------------\n";

        for (currentCycle = 0; currentCycle < totalCycles; ++currentCycle) {
            // 1. Выбираем процесс, который получит управление
            int selectedIdx = selectNextProcess();
            if (selectedIdx == -1) continue;

            Process& selected = processes[selectedIdx];
            
            // 2. Правила доступа к файлу
            bool canAccess = false;
            
            if (selected.type == 'R') {
                // Чтение разрешено, если нет активного писателя
                if (!fileBusyByWriter) {
                    canAccess = true;
                }
            } 
            else if (selected.type == 'W') {
                // Запись разрешена, если нет активных читателей и нет другого писателя
                if (activeReaders == 0 && !fileBusyByWriter) {
                    canAccess = true;
                }
            }

            // 3. Если доступ разрешён — выполняем операцию
            if (canAccess) {
                if (selected.type == 'R') {
                    // Читаем 1 символ
                    selected.charsRead++;
                    activeReaders++;
                    
                    std::cout << "[Цикл " << currentCycle << "] Процесс R" << selected.id 
                              << " читает символ (всего прочитано: " << selected.charsRead 
                              << "). Активных читателей: " << activeReaders << "\n";
                } 
                else if (selected.type == 'W') {
                    // Записываем 1 символ (файл занимается)
                    fileBusyByWriter = true;
                    currentWriterId = selected.id;
                    
                    std::cout << "[Цикл " << currentCycle << "] Процесс W" << selected.id 
                              << " начинает запись. Файл заблокирован.\n";
                }
                selected.active = true;
            } 
            else {
                std::cout << "[Цикл " << currentCycle << "] Процесс " << selected.type 
                          << selected.id << " НЕ может получить доступ (файл занят).\n";
                selected.active = false;
            }

            // 4. В конце цикла снимаем блокировки (имитация завершения операции)
            //    По условию: за один период читается/пишется 1 символ.
            //    После записи — файл освобождается.
            if (fileBusyByWriter) {
                fileBusyByWriter = false;
                std::cout << "[Цикл " << currentCycle << "] Запись завершена. Файл освобождён.\n";
                currentWriterId = -1;
            }
            
            // Читатели выходят после операции (каждый цикл — новая операция чтения)
            activeReaders = 0;
        }
        
        printResults();
    }

    void printResults() {
        std::cout << "\n=== РЕЗУЛЬТАТЫ ===\n";
        std::cout << std::left << std::setw(10) << "Процесс" 
                  << std::setw(10) << "Тип" 
                  << std::setw(10) << "Приоритет" 
                  << std::setw(20) << "Прочитано символов" << "\n";
        for (const auto& p : processes) {
            if (p.type == 'R') {
                std::cout << std::setw(10) << ("R" + std::to_string(p.id))
                          << std::setw(10) << "Чтение"
                          << std::setw(10) << p.priority
                          << std::setw(20) << p.charsRead << "\n";
            } else {
                std::cout << std::setw(10) << ("W" + std::to_string(p.id))
                          << std::setw(10) << "Запись"
                          << std::setw(10) << p.priority
                          << std::setw(20) << "—" << "\n";
            }
        }
    }

    // Получение данных для графиков (зависимость числа прочитанных символов от приоритета писателя)
    std::vector<std::pair<int, int>> getReaderStats() const {
        std::vector<std::pair<int, int>> stats;
        for (const auto& p : processes) {
            if (p.type == 'R') {
                stats.push_back({p.priority, p.charsRead});
            }
        }
        return stats;
    }
};

int main() {
    // Пример конфигурации:
    // 3 процесса чтения с разными приоритетами
    // 1 процесс записи с заданным приоритетом (будем менять)
    
    std::cout << "Лабораторная работа №3\n";
    std::cout << "Моделирование управления параллельными процессами\n\n";
    
    // Базовые процессы чтения
    std::vector<Process> baseReaders = {
        {1, 'R', 3, 0, false},
        {2, 'R', 5, 0, false},
        {3, 'R', 2, 0, false}
    };
    
    // Исследуем зависимость от приоритета писателя
    std::vector<int> writerPriorities = {1, 3, 5, 7, 10};
    
    std::cout << "Зависимость числа прочитанных символов от приоритета процесса записи:\n";
    std::cout << "------------------------------------------------------------\n";
    
    for (int wp : writerPriorities) {
        std::vector<Process> processes = baseReaders;
        processes.push_back({4, 'W', wp, 0, false});
        
        FileAccessModel model(processes, 200, 12345);  // 200 циклов
        model.run();
        
        auto stats = model.getReaderStats();
        std::cout << "\nПриоритет писателя = " << wp << ":\n";
        for (const auto& [prio, chars] : stats) {
            std::cout << "  Читатель с приоритетом " << prio << " прочитал " << chars << " симв.\n";
        }
        std::cout << "------------------------------------------------------------\n";
    }
    
    return 0;
}
