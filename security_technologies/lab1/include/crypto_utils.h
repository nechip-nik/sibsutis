#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <gmp.h>
#include <string>
#include <iostream>

// Структура для хранения параметров
struct DHParams {
    mpz_t p;
    mpz_t q;
    mpz_t g;
    
    DHParams();
    ~DHParams();
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename);
    void print() const;
};

// Структура для замера времени
struct TimingResult {
    unsigned long long key_generation;
    unsigned long long key_exchange;
    unsigned long long total;
};

// Функции генерации параметров
void generate_multiplicative_group_params(DHParams& params, int bit_length = 256);
void generate_subgroup_params(DHParams& params, int bit_length = 256);

// Функции протоколов
void diffie_hellman_original(const DHParams& params, TimingResult& timing, bool verbose = true);
void diffie_hellman_subgroup(const DHParams& params, TimingResult& timing, bool verbose = true);
void mqv_protocol(const DHParams& params, TimingResult& timing, bool verbose = true);

// Вспомогательные функции
void print_timing(const std::string& algorithm, const TimingResult& timing);
void print_comparison_table(const TimingResult dh_orig, const TimingResult dh_sub, const TimingResult mqv);

#endif // CRYPTO_UTILS_H