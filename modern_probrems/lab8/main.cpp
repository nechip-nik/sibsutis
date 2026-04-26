#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <algorithm>

// Класс, генерирующий нормально распределённые случайные числа
class NormalDistribution {
private:
    std::mt19937 rng;
    std::normal_distribution<double> dist;
public:
    NormalDistribution(double mean, double stddev, unsigned seed = 42)
        : rng(seed), dist(mean, stddev) {}
    
    double next() {
        double val = dist(rng);
        return std::max(0.1, val); // не меньше 0.1 такта
    }
};

// Синхронный конвейер
class SynchronousPipeline {
private:
    int stages;                 // количество ступеней
    int numElements;            // количество элементов в массиве
    std::vector<double> stageTime; // время обработки одного элемента на каждой ступени (фиксированное)
    
public:
    SynchronousPipeline(int s, int n, const std::vector<double>& times)
        : stages(s), numElements(n), stageTime(times) {}
    
    double run() {
        // Синхронный конвейер: скорость определяется самой медленной ступенью
        double maxStageTime = *std::max_element(stageTime.begin(), stageTime.end());
        
        // Время обработки одного элемента = сумма времён всех ступеней
        double timePerElement = 0;
        for (double t : stageTime) timePerElement += t;
        
        // С учётом конвейеризации: первый элемент идёт timePerElement,
        // остальные выходят каждые maxStageTime тактов
        double totalTime = timePerElement + (numElements - 1) * maxStageTime;
        
        return totalTime;
    }
};

// Асинхронный конвейер с буферами
class AsynchronousPipeline {
private:
    int stages;                     // количество ступеней
    int numElements;                // количество элементов
    std::vector<double> stageMean;  // среднее время обработки на каждой ступени
    std::vector<double> stageStddev;// стандартное отклонение
    
    std::vector<NormalDistribution> generators; // генераторы для каждой ступени
    
    // Состояние конвейера
    std::vector<int> stageElement;      // какой элемент сейчас на ступени (-1 если свободна)
    std::vector<double> stageRemaining; // осталось тактов до завершения на ступени
    std::vector<bool> stageBusy;        // занята ли ступень
    
    std::vector<int> inputBuffer;       // входной буфер ступени (принимает данные с предыдущей)
    std::vector<int> outputBuffer;      // выходной буфер ступени
    
    int nextElementId;                  // следующий элемент для подачи на вход
    int completedCount;                 // сколько элементов полностью обработано
    
    std::mt19937 rng;
    
public:
    AsynchronousPipeline(int s, int n, const std::vector<double>& mean, 
                         const std::vector<double>& stddev, unsigned seed = 42)
        : stages(s), numElements(n), stageMean(mean), stageStddev(stddev), rng(seed) {
        
        // Инициализируем генераторы для каждой ступени
        for (int i = 0; i < stages; i++) {
            generators.emplace_back(mean[i], stddev[i], seed + i);
        }
        
        // Инициализируем состояние
        stageElement.assign(stages, -1);
        stageRemaining.assign(stages, 0.0);
        stageBusy.assign(stages, false);
        inputBuffer.assign(stages, -1);
        outputBuffer.assign(stages, -1);
        
        nextElementId = 0;
        completedCount = 0;
    }
    
    double run() {
        double time = 0.0;
        
        // Подаём первый элемент на первую ступень
        if (stages > 0 && nextElementId < numElements) {
            stageBusy[0] = true;
            stageElement[0] = nextElementId;
            stageRemaining[0] = generators[0].next();
            nextElementId++;
        }
        
        // Основной цикл моделирования
        while (completedCount < numElements) {
            // Находим минимальное оставшееся время до следующего события
            double minRemaining = 1e9;
            for (int i = 0; i < stages; i++) {
                if (stageBusy[i] && stageRemaining[i] > 0 && stageRemaining[i] < minRemaining) {
                    minRemaining = stageRemaining[i];
                }
            }
            
            if (minRemaining >= 1e9) {
                // Нет активных ступеней — тупик (не должно случиться)
                break;
            }
            
            // Продвигаем время
            time += minRemaining;
            
            // Уменьшаем оставшееся время у всех занятых ступеней
            for (int i = 0; i < stages; i++) {
                if (stageBusy[i]) {
                    stageRemaining[i] -= minRemaining;
                    if (stageRemaining[i] < 0) stageRemaining[i] = 0;
                }
            }
            
            // Обрабатываем завершившиеся ступени
            for (int i = 0; i < stages; i++) {
                if (stageBusy[i] && stageRemaining[i] == 0) {
                    int elementId = stageElement[i];
                    
                    if (i == stages - 1) {
                        // Последняя ступень — элемент полностью обработан
                        completedCount++;
                        stageBusy[i] = false;
                        stageElement[i] = -1;
                    } else {
                        // Передаём на следующую ступень через буфер
                        // Сначала в выходной буфер текущей ступени
                        outputBuffer[i] = elementId;
                        
                        // Затем пробуем передать во входной буфер следующей ступени
                        if (inputBuffer[i + 1] == -1) {
                            inputBuffer[i + 1] = outputBuffer[i];
                            outputBuffer[i] = -1;
                        }
                        
                        stageBusy[i] = false;
                        stageElement[i] = -1;
                    }
                }
            }
            
            // Запускаем ступени, если есть данные в буферах
            for (int i = 0; i < stages; i++) {
                if (!stageBusy[i]) {
                    int elementToProcess = -1;
                    
                    if (i == 0) {
                        // Первая ступень: берём из входного потока
                        if (nextElementId < numElements) {
                            elementToProcess = nextElementId;
                            nextElementId++;
                        } else if (inputBuffer[0] != -1) {
                            elementToProcess = inputBuffer[0];
                            inputBuffer[0] = -1;
                        }
                    } else {
                        // Остальные ступени: берём из входного буфера
                        if (inputBuffer[i] != -1) {
                            elementToProcess = inputBuffer[i];
                            inputBuffer[i] = -1;
                        }
                    }
                    
                    if (elementToProcess != -1) {
                        stageBusy[i] = true;
                        stageElement[i] = elementToProcess;
                        stageRemaining[i] = generators[i].next();
                    }
                }
            }
            
            // Проверяем возможность пересылки из выходных буферов во входные
            for (int i = 0; i < stages - 1; i++) {
                if (outputBuffer[i] != -1 && inputBuffer[i + 1] == -1) {
                    inputBuffer[i + 1] = outputBuffer[i];
                    outputBuffer[i] = -1;
                }
            }
        }
        
        return time;
    }
};

// Функция для проведения эксперимента
void runExperiment(int stages, int numElements, const std::vector<double>& mean, 
                   const std::vector<double>& stddev, const std::string& label) {
    std::cout << "\n=== " << label << " ===\n";
    std::cout << "Ступеней: " << stages << ", Элементов: " << numElements << "\n";
    
    // Синхронный конвейер (время на каждой ступени = мат. ожидание)
    std::vector<double> fixedTimes(mean.begin(), mean.end());
    SynchronousPipeline sync(stages, numElements, fixedTimes);
    double syncTime = sync.run();
    
    // Асинхронный конвейер
    AsynchronousPipeline async(stages, numElements, mean, stddev);
    double asyncTime = async.run();
    
    double speedup = syncTime / asyncTime;
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Время синхронного конвейера: " << syncTime << " тактов\n";
    std::cout << "Время асинхронного конвейера: " << asyncTime << " тактов\n";
    std::cout << "Ускорение (Sync/Async): " << speedup << "x\n";
    std::cout << "----------------------------------------\n";
}

int main() {
    std::cout << "Лабораторная работа №5\n";
    std::cout << "Моделирование работы асинхронного конвейера\n";
    std::cout << "============================================\n";
    
    const int NUM_ELEMENTS = 1000;  // много элементов
    
    // Эксперимент 1: 3 ступени с одинаковыми параметрами
    {
        int stages = 3;
        std::vector<double> mean = {10.0, 10.0, 10.0};
        std::vector<double> stddev = {2.0, 2.0, 2.0};
        runExperiment(stages, NUM_ELEMENTS, mean, stddev, 
                      "3 ступени, одинаковые параметры");
    }
    
    // Эксперимент 2: 3 ступени с разными параметрами
    {
        int stages = 3;
        std::vector<double> mean = {5.0, 15.0, 8.0};
        std::vector<double> stddev = {1.0, 3.0, 2.0};
        runExperiment(stages, NUM_ELEMENTS, mean, stddev, 
                      "3 ступени, разные параметры");
    }
    
    // Эксперимент 3: 5 ступеней
    {
        int stages = 5;
        std::vector<double> mean = {8.0, 12.0, 6.0, 10.0, 7.0};
        std::vector<double> stddev = {2.0, 2.5, 1.5, 2.0, 1.8};
        runExperiment(stages, NUM_ELEMENTS, mean, stddev, 
                      "5 ступеней, разные параметры");
    }
    
    // Эксперимент 4: зависимость от числа ступеней (для графика)
    std::cout << "\n=== Зависимость ускорения от числа ступеней ===\n";
    std::cout << std::left << std::setw(15) << "Число ступеней"
              << std::setw(20) << "Ускорение (Sync/Async)" << "\n";
    std::cout << "------------------------------------------------\n";
    
    for (int stages = 1; stages <= 8; stages++) {
        std::vector<double> mean(stages, 10.0);
        std::vector<double> stddev(stages, 3.0);
        
        SynchronousPipeline sync(stages, NUM_ELEMENTS, mean);
        AsynchronousPipeline async(stages, NUM_ELEMENTS, mean, stddev);
        
        double syncTime = sync.run();
        double asyncTime = async.run();
        double speedup = syncTime / asyncTime;
        
        std::cout << std::left << std::setw(15) << stages
                  << std::setw(20) << std::fixed << std::setprecision(3) << speedup << "\n";
    }
    
    std::cout << "\n=== Вывод ===\n";
    std::cout << "Асинхронный конвейер эффективнее синхронного, когда:\n";
    std::cout << "1. Время обработки на разных ступенях существенно различается\n";
    std::cout << "2. Есть случайные колебания времени обработки\n";
    std::cout << "3. Число ступеней достаточно велико\n";
    
    return 0;
}
