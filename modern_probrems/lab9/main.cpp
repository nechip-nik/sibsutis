#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <chrono>

struct Parameters {
    double H;
    double sigma_a;
    double sigma_s;
    long long N;
    double sigma;
    double p_absorb;

    Parameters(double h, double sa, double ss, long long n)
        : H(h), sigma_a(sa), sigma_s(ss), N(n) {
        sigma = sigma_a + sigma_s;
        p_absorb = sigma_a / sigma;
    }
};

class NeutronTransportModel {
private:
    Parameters params;
    std::mt19937 gen;
    std::uniform_real_distribution<double> dist;

    long long passed = 0;
    long long reflected = 0;
    long long absorbed = 0;

    double random() {
        return dist(gen);
    }

    double sample_free_path() {
        // Формула (7.1): λ = -1/Σ * ln(1 - γ)
        return -std::log(1.0 - random()) / params.sigma;
    }

    double sample_cos_phi() {
        // Формула (7.3): cos φ = 1 - 2γ  (равномерно на [-1, 1])
        return 1.0 - 2.0 * random();
    }

    void simulate_single_neutron() {
        double x = 0.0;
        bool first_step = true; // Флаг первого шага

        while (true) {
            double lambda = sample_free_path();
            double cos_phi;
            
            // !!! ВАЖНОЕ ИСПРАВЛЕНИЕ !!!
            // По условию задачи нейтроны падают ПЕРПЕНДИКУЛЯРНО пластинке.
            // Значит первый шаг всегда направлен строго вправо (cos = 1).
            if (first_step) {
                cos_phi = 1.0;
                first_step = false;
            } else {
                cos_phi = sample_cos_phi();
            }

            double x_new = x + lambda * cos_phi;

            // Проверка границ
            if (x_new > params.H) {
                passed++;
                return;
            }
            if (x_new < 0.0) {
                reflected++;
                return;
            }

            // Проверка поглощения
            if (random() <= params.p_absorb) {
                absorbed++;
                return;
            }

            // Рассеяние внутри пластинки - продолжаем движение
            x = x_new;
        }
    }

public:
    NeutronTransportModel(const Parameters& p) : params(p) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        gen.seed(seed);
        dist = std::uniform_real_distribution<double>(0.0, 1.0);
    }

    void run() {
        passed = reflected = absorbed = 0;

        std::cout << "=== Моделирование прохождения нейтронов (Метод Монте-Карло) ===" << std::endl;
        std::cout << std::fixed << std::setprecision(4);
        std::cout << "Параметры: H=" << params.H 
                  << ", Σa=" << params.sigma_a 
                  << ", Σs=" << params.sigma_s << std::endl;
        std::cout << "Запуск " << params.N << " нейтронов..." << std::endl;

        long long progress_step = params.N / 10;
        for (long long i = 0; i < params.N; ++i) {
            simulate_single_neutron();
            if ((i + 1) % progress_step == 0) {
                std::cout << "  Обработано: " << (i + 1) << " / " << params.N << std::endl;
            }
        }

        print_results();
    }

    void print_results() const {
        double p_pass = static_cast<double>(passed) / params.N;
        double p_refl = static_cast<double>(reflected) / params.N;
        double p_abs  = static_cast<double>(absorbed) / params.N;

        std::cout << "\n--- Результаты моделирования ---" << std::endl;
        std::cout << "Всего частиц: " << params.N << std::endl;
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Прошло сквозь пластинку: " << passed << " (P = " << p_pass << ")" << std::endl;
        std::cout << "Отразилось:             " << reflected << " (P = " << p_refl << ")" << std::endl;
        std::cout << "Поглотилось:            " << absorbed << " (P = " << p_abs << ")" << std::endl;
        std::cout << "Сумма вероятностей:     " << (p_pass + p_refl + p_abs) << std::endl;
    }
};

int main() {
    Parameters params(3.0, 0.2, 0.8, 1000000);
    NeutronTransportModel model(params);
    model.run();
    return 0;
}