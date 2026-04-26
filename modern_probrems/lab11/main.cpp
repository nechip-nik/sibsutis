#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <chrono>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <memory>

// Параметры моделирования
const double LAMBDA = 1.0;           // интенсивность входного потока
const double MU = 0.95;              // интенсивность обслуживания
const int TOTAL_CYCLES = 100000;     // количество тактов моделирования
const int QUEUE_LOG_INTERVAL = 100;  // интервал записи длины очереди
const int ERLANG_ORDER = 3;          // порядок потока Эрланга (можно менять)

// Генератор случайных чисел
std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_real_distribution<double> dist(0.0, 1.0);

double rnd() {
    return dist(gen);
}

// Экспоненциальное распределение
double exponential(double rate) {
    return -std::log(1.0 - rnd()) / rate;
}

// ========== Генераторы входных потоков ==========

// Базовый класс генератора потока требований
class ArrivalGenerator {
public:
    virtual ~ArrivalGenerator() = default;
    virtual bool has_arrival(double dt = 1.0) = 0;
    virtual std::string get_name() const = 0;
};

// 1. Регулярный (детерминированный) поток
// Требования приходят строго через интервал 1/λ
class RegularGenerator : public ArrivalGenerator {
private:
    double time_accumulator;
    double interval;
    
public:
    RegularGenerator(double lambda) : time_accumulator(0.0) {
        interval = 1.0 / lambda;
    }
    
    bool has_arrival(double dt = 1.0) override {
        time_accumulator += dt;
        if (time_accumulator >= interval) {
            time_accumulator -= interval;
            return true;
        }
        return false;
    }
    
    std::string get_name() const override {
        return "Регулярный";
    }
};

// 2. Пуассоновский (простейший) поток
// Интервалы между приходами распределены экспоненциально
class PoissonGenerator : public ArrivalGenerator {
private:
    double lambda;
    double time_to_next;
    
public:
    PoissonGenerator(double lambda) : lambda(lambda) {
        time_to_next = exponential(lambda);
    }
    
    bool has_arrival(double dt = 1.0) override {
        time_to_next -= dt;
        if (time_to_next <= 0.0) {
            // Генерируем время до следующего прихода
            time_to_next = exponential(lambda);
            return true;
        }
        return false;
    }
    
    std::string get_name() const override {
        return "Пуассоновский";
    }
};

// 3. Поток Эрланга k-го порядка
// Сумма k экспоненциально распределённых величин
class ErlangGenerator : public ArrivalGenerator {
private:
    double lambda;
    int k;              // порядок потока
    double time_to_next;
    
    double erlang_time() {
        // Сумма k экспоненциальных величин с параметром k*lambda
        double sum = 0.0;
        for (int i = 0; i < k; ++i) {
            sum += exponential(k * lambda);
        }
        return sum;
    }
    
public:
    ErlangGenerator(double lambda, int order) : lambda(lambda), k(order) {
        time_to_next = erlang_time();
    }
    
    bool has_arrival(double dt = 1.0) override {
        time_to_next -= dt;
        if (time_to_next <= 0.0) {
            time_to_next = erlang_time();
            return true;
        }
        return false;
    }
    
    std::string get_name() const override {
        return "Эрланга " + std::to_string(k) + "-го порядка";
    }
};

// ========== Прибор обслуживания ==========

class Server {
private:
    double mu;
    double remaining_time;
    bool busy;

public:
    Server(double service_rate) : mu(service_rate), remaining_time(0.0), busy(false) {}

    void start_service() {
        busy = true;
        remaining_time = exponential(mu);
    }

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

// ========== Система массового обслуживания ==========

class QueueingSystem {
private:
    std::unique_ptr<ArrivalGenerator> generator;
    Server server;
    std::queue<int> queue;
    
    int queue_length;
    int total_served;
    int total_arrived;
    
    std::vector<int> queue_history;
    std::vector<int> time_points;

public:
    QueueingSystem(std::unique_ptr<ArrivalGenerator> gen, double mu)
        : generator(std::move(gen)), server(mu), queue_length(0), 
          total_served(0), total_arrived(0) {}

    void run(int total_cycles, int log_interval) {
        for (int cycle = 0; cycle < total_cycles; ++cycle) {
            // 1. Проверка прихода требования
            if (generator->has_arrival()) {
                total_arrived++;
                
                if (!server.is_busy()) {
                    server.start_service();
                } else {
                    queue.push(total_arrived);
                    queue_length++;
                }
            }

            // 2. Обслуживание
            bool was_busy = server.is_busy();
            server.tick();
            
            if (was_busy && !server.is_busy()) {
                total_served++;
                
                // Если есть очередь — начинаем обслуживание следующего
                if (!queue.empty()) {
                    queue.pop();
                    queue_length--;
                    server.start_service();
                }
            }

            // 3. Сбор статистики
            if (cycle % log_interval == 0) {
                time_points.push_back(cycle);
                queue_history.push_back(queue_length);
            }
        }
    }

    std::string get_generator_name() const {
        return generator->get_name();
    }

    int get_queue_length() const { return queue_length; }
    int get_total_served() const { return total_served; }
    int get_total_arrived() const { return total_arrived; }

    const std::vector<int>& get_queue_history() const { return queue_history; }
    const std::vector<int>& get_time_points() const { return time_points; }

    double get_avg_queue_length() const {
        if (queue_history.empty()) return 0.0;
        double sum = 0.0;
        for (int q : queue_history) sum += q;
        return sum / queue_history.size();
    }
};

// ========== Вспомогательные функции ==========

void print_separator(char ch = '=', int width = 60) {
    std::cout << std::string(width, ch) << std::endl;
}

void print_system_stats(QueueingSystem& sys) {
    std::cout << "Система с " << sys.get_generator_name() << " потоком:" << std::endl;
    std::cout << "  Поступило требований:  " << sys.get_total_arrived() << std::endl;
    std::cout << "  Обслужено требований:  " << sys.get_total_served() << std::endl;
    std::cout << "  Осталось в очереди:    " << sys.get_queue_length() << std::endl;
    std::cout << "  Средняя длина очереди: " << std::fixed << std::setprecision(3) 
              << sys.get_avg_queue_length() << std::endl;
    if (sys.get_total_arrived() > 0) {
        std::cout << "  Доля обслуженных:      " << std::fixed << std::setprecision(4)
                  << (double)sys.get_total_served() / sys.get_total_arrived() << std::endl;
    }
    std::cout << std::endl;
}

void save_results_to_csv(const std::vector<QueueingSystem>& systems) {
    std::ofstream file("queue_length_comparison.csv");
    
    // Заголовок
    file << "Time";
    for (const auto& sys : systems) {
        file << "," << sys.get_generator_name();
    }
    file << std::endl;

    // Данные
    const auto& time_points = systems[0].get_time_points();
    for (size_t i = 0; i < time_points.size(); ++i) {
        file << time_points[i];
        for (const auto& sys : systems) {
            file << "," << sys.get_queue_history()[i];
        }
        file << std::endl;
    }
    
    file.close();
    std::cout << "Данные сохранены в файл 'queue_length_comparison.csv'" << std::endl;
}

// ========== Главная функция ==========

int main() {
    std::cout << "=== Лабораторная работа №10 ===" << std::endl;
    std::cout << "Изучение потоков требований" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Параметры моделирования:" << std::endl;
    std::cout << "  Интенсивность входного потока λ = " << LAMBDA << std::endl;
    std::cout << "  Интенсивность обслуживания μ = " << MU << std::endl;
    std::cout << "  Коэффициент загрузки ρ = " << LAMBDA / MU << std::endl;
    std::cout << "  Количество тактов: " << TOTAL_CYCLES << std::endl;
    std::cout << "  Порядок потока Эрланга: k = " << ERLANG_ORDER << std::endl;
    std::cout << std::endl;

    // Создаём системы с разными генераторами
    std::vector<QueueingSystem> systems;
    
    std::cout << "Создание систем..." << std::endl;
    systems.emplace_back(std::make_unique<RegularGenerator>(LAMBDA), MU);
    systems.emplace_back(std::make_unique<PoissonGenerator>(LAMBDA), MU);
    systems.emplace_back(std::make_unique<ErlangGenerator>(LAMBDA, ERLANG_ORDER), MU);
    std::cout << std::endl;

    // Запускаем моделирование для каждой системы
    for (size_t i = 0; i < systems.size(); ++i) {
        std::cout << "Моделирование системы " << (i + 1) << "/" << systems.size();
        std::cout << " (" << systems[i].get_generator_name() << " поток)..." << std::endl;
        
        systems[i].run(TOTAL_CYCLES, QUEUE_LOG_INTERVAL);
    }
    std::cout << std::endl;

    // Вывод результатов
    print_separator('=');
    std::cout << "РЕЗУЛЬТАТЫ МОДЕЛИРОВАНИЯ" << std::endl;
    print_separator('=');
    std::cout << std::endl;

    for (auto& sys : systems) {
        print_system_stats(sys);
    }

    print_separator('-');
    std::cout << "СРАВНЕНИЕ СРЕДНИХ ДЛИН ОЧЕРЕДЕЙ" << std::endl;
    print_separator('-');
    
    for (auto& sys : systems) {
        std::cout << "  " << std::left << std::setw(25) << sys.get_generator_name() + ":";
        std::cout << std::fixed << std::setprecision(3) << sys.get_avg_queue_length() << std::endl;
    }
    std::cout << std::endl;

    // Сохранение данных для построения графиков
    save_results_to_csv(systems);

    // Вывод теоретического сравнения
    print_separator('=');
    std::cout << "ТЕОРЕТИЧЕСКИЙ АНАЛИЗ" << std::endl;
    print_separator('=');
    
    double rho = LAMBDA / MU;
    std::cout << "Коэффициент загрузки ρ = λ/μ = " << rho << std::endl;
    
    if (rho >= 1.0) {
        std::cout << "ВНИМАНИЕ: ρ >= 1, очередь будет расти неограниченно!" << std::endl;
    } else {
        // Теоретическая средняя длина очереди для M/M/1
        double Lq_mm1 = (rho * rho) / (1 - rho);
        std::cout << "Теоретическая средняя длина очереди для M/M/1: " << Lq_mm1 << std::endl;
        
        // Для M/D/1 (регулярный поток)
        double Lq_md1 = (rho * rho) / (2 * (1 - rho));
        std::cout << "Теоретическая средняя длина очереди для M/D/1: " << Lq_md1 << std::endl;
        
        // Для M/E_k/1 (поток Эрланга)
        double Lq_mek1 = ((rho * rho) / (2 * (1 - rho))) * (1 + 1.0 / ERLANG_ORDER);
        std::cout << "Теоретическая средняя длина очереди для M/E_" << ERLANG_ORDER << "/1: " 
                  << Lq_mek1 << std::endl;
    }

    return 0;
}