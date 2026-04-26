#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <chrono>
#include <vector>
#include <queue>
#include <fstream>

// Параметры моделирования
const double LAMBDA = 1.0;           // интенсивность входного потока
const double MU1 = 0.9;              // интенсивность обслуживания (1 прибор)
const double MU3_SINGLE = 0.3;       // интенсивность одного прибора (3 прибора)
const int TOTAL_CYCLES = 100000;     // количество тактов моделирования
const int QUEUE_LOG_INTERVAL = 100;  // интервал записи длины очереди

// Генератор случайных чисел (Вихрь Мерсенна)
std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_real_distribution<double> dist(0.0, 1.0);

// !!! ИСПРАВЛЕНО: переименовано в rnd() во избежание конфликта с stdlib.h !!!
double rnd() {
    return dist(gen);
}

// Экспоненциальное распределение: f(t) = rate * exp(-rate * t)
double exponential(double rate) {
    return -std::log(1.0 - rnd()) / rate;
}

// Пуассоновский поток: вероятность прихода требования за время dt
bool poisson_arrival(double lambda, double dt = 1.0) {
    return rnd() < (1.0 - std::exp(-lambda * dt));
}

// Класс "Прибор обслуживания"
class Server {
private:
    double mu;                  // интенсивность обслуживания
    double remaining_time;      // оставшееся время до завершения
    bool busy;                  // занят ли прибор

public:
    Server(double service_rate) : mu(service_rate), remaining_time(0.0), busy(false) {}

    // Начать обслуживание нового требования
    void start_service() {
        busy = true;
        remaining_time = exponential(mu);
    }

    // Обновить состояние прибора (прошло dt времени)
    void tick(double dt = 1.0) {
        if (busy) {
            remaining_time -= dt;
            if (remaining_time <= 0.0) {
                busy = false;
                remaining_time = 0.0;
            }
        }
    }

    bool is_busy() const { return busy; }
};

// Базовый класс СМО
class QueueingSystem {
protected:
    std::vector<Server> servers;
    std::queue<int> queue;      // очередь требований
    int queue_length;           // текущая длина очереди
    int total_served;           // обслужено всего
    int total_arrived;          // поступило всего

public:
    QueueingSystem(int num_servers, double mu_per_server) 
        : queue_length(0), total_served(0), total_arrived(0) {
        for (int i = 0; i < num_servers; ++i) {
            servers.emplace_back(mu_per_server);
        }
    }

    virtual void process_arrival() {
        total_arrived++;
        
        // Ищем свободный прибор
        int free_server = -1;
        for (size_t i = 0; i < servers.size(); ++i) {
            if (!servers[i].is_busy()) {
                free_server = i;
                break;
            }
        }

        if (free_server != -1) {
            // Есть свободный прибор — начинаем обслуживание сразу
            servers[free_server].start_service();
        } else {
            // Все приборы заняты — в очередь
            queue.push(total_arrived);
            queue_length++;
        }
    }

    virtual void tick() {
        // Обновляем состояние всех приборов
        for (auto& server : servers) {
            bool was_busy = server.is_busy();
            server.tick();
            
            // Если прибор освободился — пытаемся взять требование из очереди
            if (was_busy && !server.is_busy() && !queue.empty()) {
                queue.pop();
                queue_length--;
                server.start_service();
                total_served++;
            } else if (was_busy && !server.is_busy()) {
                total_served++;
            }
        }
    }

    int get_queue_length() const { return queue_length; }
    int get_total_served() const { return total_served; }
    int get_total_arrived() const { return total_arrived; }

    virtual std::string get_name() const = 0;
};

// СМО с одним прибором
class SingleServerSystem : public QueueingSystem {
public:
    SingleServerSystem() : QueueingSystem(1, MU1) {}
    
    std::string get_name() const override {
        return "1 прибор (mu = 0.9)";
    }
};

// СМО с тремя приборами
class ThreeServerSystem : public QueueingSystem {
public:
    ThreeServerSystem() : QueueingSystem(3, MU3_SINGLE) {}
    
    std::string get_name() const override {
        return "3 прибора (mu = 0.3 каждый)";
    }
};

// Структура для хранения статистики
struct Statistics {
    std::vector<int> queue_history_1;
    std::vector<int> queue_history_3;
    std::vector<int> time_points;
};

int main() {
    std::cout << "=== Лабораторная работа №9 ===" << std::endl;
    std::cout << "Моделирование систем массового обслуживания" << std::endl;
    std::cout << std::endl;
    std::cout << "Параметры моделирования:" << std::endl;
    std::cout << "  Интенсивность входного потока λ = " << LAMBDA << std::endl;
    std::cout << "  Система 1: 1 прибор, μ = " << MU1;
    std::cout << " (коэфф. загрузки ρ = " << LAMBDA / MU1 << ")" << std::endl;
    std::cout << "  Система 2: 3 прибора, μ = " << MU3_SINGLE << " каждый";
    std::cout << " (коэфф. загрузки ρ = " << LAMBDA / (3 * MU3_SINGLE) << ")" << std::endl;
    std::cout << "  Количество тактов: " << TOTAL_CYCLES << std::endl;
    std::cout << std::endl;

    // Создаём системы
    SingleServerSystem sys1;
    ThreeServerSystem sys3;

    Statistics stats;
    
    std::cout << "Запуск моделирования..." << std::endl;

    // Главный цикл моделирования
    for (int cycle = 0; cycle < TOTAL_CYCLES; ++cycle) {
        // 1. Генерация входного потока (одинаковый для обеих систем)
        bool arrival = poisson_arrival(LAMBDA);
        
        if (arrival) {
            sys1.process_arrival();
            sys3.process_arrival();
        }

        // 2. Обработка требований (такт работы)
        sys1.tick();
        sys3.tick();

        // 3. Сбор статистики
        if (cycle % QUEUE_LOG_INTERVAL == 0) {
            stats.time_points.push_back(cycle);
            stats.queue_history_1.push_back(sys1.get_queue_length());
            stats.queue_history_3.push_back(sys3.get_queue_length());
        }

        // Вывод прогресса
        if ((cycle + 1) % (TOTAL_CYCLES / 10) == 0) {
            std::cout << "  Прогресс: " << (cycle + 1) << " / " << TOTAL_CYCLES;
            std::cout << " | Очередь 1: " << sys1.get_queue_length();
            std::cout << " | Очередь 3: " << sys3.get_queue_length() << std::endl;
        }
    }

    // Вывод итоговой статистики
    std::cout << std::endl;
    std::cout << "========== РЕЗУЛЬТАТЫ МОДЕЛИРОВАНИЯ ==========" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Система 1 (" << sys1.get_name() << "):" << std::endl;
    std::cout << "  Поступило требований:  " << sys1.get_total_arrived() << std::endl;
    std::cout << "  Обслужено требований:  " << sys1.get_total_served() << std::endl;
    std::cout << "  Осталось в очереди:    " << sys1.get_queue_length() << std::endl;
    if (sys1.get_total_arrived() > 0) {
        std::cout << "  Доля обслуженных:      " 
                  << std::fixed << std::setprecision(4)
                  << (double)sys1.get_total_served() / sys1.get_total_arrived() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Система 2 (" << sys3.get_name() << "):" << std::endl;
    std::cout << "  Поступило требований:  " << sys3.get_total_arrived() << std::endl;
    std::cout << "  Обслужено требований:  " << sys3.get_total_served() << std::endl;
    std::cout << "  Осталось в очереди:    " << sys3.get_queue_length() << std::endl;
    if (sys3.get_total_arrived() > 0) {
        std::cout << "  Доля обслуженных:      " 
                  << std::fixed << std::setprecision(4)
                  << (double)sys3.get_total_served() / sys3.get_total_arrived() << std::endl;
    }
    std::cout << std::endl;

    // Сохранение данных для построения графиков
    std::ofstream data_file("queue_length_data.csv");
    data_file << "Time,Queue1,Queue3" << std::endl;
    for (size_t i = 0; i < stats.time_points.size(); ++i) {
        data_file << stats.time_points[i] << ","
                  << stats.queue_history_1[i] << ","
                  << stats.queue_history_3[i] << std::endl;
    }
    data_file.close();
    
    std::cout << "Данные сохранены в файл 'queue_length_data.csv'" << std::endl;
    std::cout << "================================================" << std::endl;

    // Вывод средних длин очередей
    double avg_queue1 = 0.0, avg_queue3 = 0.0;
    for (size_t i = 0; i < stats.queue_history_1.size(); ++i) {
        avg_queue1 += stats.queue_history_1[i];
        avg_queue3 += stats.queue_history_3[i];
    }
    avg_queue1 /= stats.queue_history_1.size();
    avg_queue3 /= stats.queue_history_3.size();

    std::cout << std::endl;
    std::cout << "Средняя длина очереди:" << std::endl;
    std::cout << "  Система 1: " << std::fixed << std::setprecision(3) << avg_queue1 << std::endl;
    std::cout << "  Система 2: " << std::fixed << std::setprecision(3) << avg_queue3 << std::endl;

    return 0;
}