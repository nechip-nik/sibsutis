#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <stdexcept>

using namespace std;

// Функция для расчета оперативной надежности R*(t)
double calculateRStar(double lambda, int n, double t) {
    if (lambda <= 0 || n <= 0 || t < 0) 
        throw invalid_argument("Некорректные параметры для R*(t)");
    return exp(-lambda * n * t);
}

// Функция для расчета оперативной восстановимости U*(t)
double calculateUStar(double mu, int m, double t) {
    if (mu <= 0 || m <= 0 || t < 0) 
        throw invalid_argument("Некорректные параметры для U*(t)");
    return 1 - exp(-mu * m * t);
}

// Функция для расчета коэффициента готовности S
double calculateS(double lambda, double mu, int n, int m) {
    if (lambda <= 0 || mu <= 0 || n <= 0 || m <= 0) 
        throw invalid_argument("Некорректные параметры для S");
    return (mu * m) / (lambda * n + mu * m);
}

void generateData(int N, double lambda, double mu, int m, int t_max) {
    // Генерация данных для R*(t)
    ofstream Rfile("R_star_data.txt");
    Rfile << "t\tR*(t) n=8\tR*(t) n=9\tR*(t) n=10\n";
    for (int t = 0; t <= t_max; t += 2) {
        Rfile << t << "\t"
              << calculateRStar(lambda, 8, t) << "\t"
              << calculateRStar(lambda, 9, t) << "\t"
              << calculateRStar(lambda, 10, t) << "\n";
    }
    Rfile.close();

    // Генерация данных для U*(t)
    ofstream Ufile("U_star_data.txt");
    Ufile << "t\t";
    for (int n = 10; n <= 16; ++n) Ufile << "U*(t) n=" << n << "\t";
    Ufile << "\n";
    for (int t = 0; t <= t_max; t += 2) {
        Ufile << t;
        for (int n = 10; n <= 16; ++n) {
            Ufile << "\t" << calculateUStar(mu, m, t);
        }
        Ufile << "\n";
    }
    Ufile.close();

    // Генерация таблицы S
    ofstream Sfile("S_values.txt");
    Sfile << "n\tS\n";
    for (int n = 11; n <= 16; ++n) {
        Sfile << n << "\t" << fixed << setprecision(3) 
              << calculateS(lambda, mu, n, m) << "\n";
    }
    Sfile.close();
}

void generateGnuplotScripts() {
    // Скрипт для R*(t)
    ofstream Rscript("plot_R_star.gnu");
    Rscript << "set terminal pngcairo enhanced font 'Arial,12'\n"
            << "set output 'R_star_plot.png'\n"
            << "set title 'Зависимость R*(t) для различных n'\n"
            << "set xlabel 'Время t (часы)'\n"
            << "set ylabel 'R*(t)'\n"
            << "set grid\n"
            << "plot 'R_star_data.txt' using 1:2 with linespoints title 'n=8', \\\n"
            << "     '' using 1:3 with linespoints title 'n=9', \\\n"
            << "     '' using 1:4 with linespoints title 'n=10'\n";
    Rscript.close();

    // Скрипт для U*(t)
    ofstream Uscript("plot_U_star.gnu");
    Uscript << "set terminal pngcairo enhanced font 'Arial,12'\n"
            << "set output 'U_star_plot.png'\n"
            << "set title 'Зависимость U*(t) для различных n'\n"
            << "set xlabel 'Время t (часы)'\n"
            << "set ylabel 'U*(t)'\n"
            << "set grid\n"
            << "plot for [col=2:8] 'U_star_data.txt' using 1:col with lines title columnheader\n";
    Uscript.close();
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 6) throw runtime_error("Использование: ./program N lambda mu m t_max");
        
        int N = stoi(argv[1]);
        double lambda = stod(argv[2]);
        double mu = stod(argv[3]);
        int m = stoi(argv[4]);
        int t_max = stoi(argv[5]);

        if (N < 1 || lambda <= 0 || mu <= 0 || m < 1 || t_max < 0)
            throw invalid_argument("Некорректные значения параметров");

        generateData(N, lambda, mu, m, t_max);
        generateGnuplotScripts();

        cout << "Данные и скрипты успешно сгенерированы.\n"
             << "Для построения графиков выполните:\n"
             << "gnuplot plot_R_star.gnu\n"
             << "gnuplot plot_U_star.gnu\n";

    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }
    return 0;
}