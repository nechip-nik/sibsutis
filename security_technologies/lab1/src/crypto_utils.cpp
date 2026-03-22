#include "crypto_utils.h"
#include "rdtsc.h"
#include <fstream>
#include <gmp.h>

// Конструктор
DHParams::DHParams() {
    mpz_init(p);
    mpz_init(q);
    mpz_init(g);
}

// Деструктор
DHParams::~DHParams() {
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(g);
}

// Загрузка из файла
void DHParams::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return;
    }
    
    char buffer[4096];
    
    file.getline(buffer, sizeof(buffer));
    mpz_set_str(p, buffer, 10);
    
    file.getline(buffer, sizeof(buffer));
    mpz_set_str(q, buffer, 10);
    
    file.getline(buffer, sizeof(buffer));
    mpz_set_str(g, buffer, 10);
    
    file.close();
}

// Сохранение в файл
void DHParams::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось создать файл " << filename << std::endl;
        return;
    }
    
    file << mpz_get_str(NULL, 10, p) << "\n";
    file << mpz_get_str(NULL, 10, q) << "\n";
    file << mpz_get_str(NULL, 10, g) << "\n";
    
    file.close();
}

// Вывод параметров
void DHParams::print() const {
    std::cout << "p = "; mpz_out_str(stdout, 10, p); std::cout << "\n";
    std::cout << "q = "; mpz_out_str(stdout, 10, q); std::cout << "\n";
    std::cout << "g = "; mpz_out_str(stdout, 10, g); std::cout << "\n";
}

// Генерация параметров для мультипликативной группы Zp*
void generate_multiplicative_group_params(DHParams& params, int bit_length) {
    gmp_randstate_t state;
    gmp_randinit_default(state);
    
    mpz_t temp;
    mpz_init(temp);
    
    std::cout << "Генерация параметров для мультипликативной группы Zp* (" << bit_length << " бит)...\n";
    
    do {
        mpz_urandomb(params.q, state, bit_length);
        mpz_nextprime(params.q, params.q);
        
        // p = 2q + 1
        mpz_mul_ui(params.p, params.q, 2);
        mpz_add_ui(params.p, params.p, 1);
    } while (!mpz_probab_prime_p(params.p, 25));
    
    // Ищем генератор g (g^q mod p ≠ 1)
    do {
        mpz_urandomm(params.g, state, params.p);
        if (mpz_cmp_ui(params.g, 1) <= 0) continue;
        
        mpz_powm(temp, params.g, params.q, params.p);
    } while (mpz_cmp_ui(temp, 1) == 0);
    
    mpz_clear(temp);
    gmp_randclear(state);
    
    std::cout << "Параметры сгенерированы:\n";
    params.print();
    std::cout << std::endl;
}

// Генерация параметров для циклической подгруппы порядка q
void generate_subgroup_params(DHParams& params, int bit_length) {
    gmp_randstate_t state;
    gmp_randinit_default(state);
    
    mpz_t t, r, check;
    mpz_init(t);
    mpz_init(r);
    mpz_init(check);
    
    std::cout << "Генерация параметров для циклической подгруппы (" << bit_length << " бит)...\n";
    
    // Генерируем простое q
    mpz_urandomb(params.q, state, bit_length);
    mpz_nextprime(params.q, params.q);
    
    // Генерируем простое p такое, что q делит (p-1)
    do {
        mpz_urandomb(t, state, bit_length/2);
        if (mpz_cmp_ui(t, 0) == 0) mpz_set_ui(t, 1);
        
        // p = t*q + 1
        mpz_mul(params.p, t, params.q);
        mpz_add_ui(params.p, params.p, 1);
        
        if (mpz_probab_prime_p(params.p, 25) > 0) {
            break;
        }
    } while (true);
    
    // Ищем генератор подгруппы порядка q
    do {
        mpz_urandomm(r, state, params.p);
        mpz_powm(params.g, r, t, params.p);
    } while (mpz_cmp_ui(params.g, 1) == 0);
    
    // Проверка: g^q mod p должно быть равно 1
    mpz_powm(check, params.g, params.q, params.p);
    
    std::cout << "Параметры сгенерированы:\n";
    params.print();
    std::cout << "Проверка g^q mod p = "; 
    mpz_out_str(stdout, 10, check); 
    std::cout << " (должно быть 1)" << std::endl << std::endl;
    
    mpz_clear(t);
    mpz_clear(r);
    mpz_clear(check);
    gmp_randclear(state);
}

// Оригинальный протокол Диффи-Хеллмана
void diffie_hellman_original(const DHParams& params, TimingResult& timing, bool verbose) {
    mpz_t secret_a, secret_b;
    mpz_t public_a, public_b;
    mpz_t shared_a, shared_b;
    
    mpz_init(secret_a); mpz_init(secret_b);
    mpz_init(public_a); mpz_init(public_b);
    mpz_init(shared_a); mpz_init(shared_b);
    
    gmp_randstate_t state;
    gmp_randinit_default(state);
    
    // Генерация ключей
    unsigned long long start_keygen = rdtsc();
    
    mpz_urandomm(secret_a, state, params.p);
    mpz_urandomm(secret_b, state, params.p);
    mpz_powm(public_a, params.g, secret_a, params.p);
    mpz_powm(public_b, params.g, secret_b, params.p);
    
    unsigned long long end_keygen = rdtsc();
    
    // Обмен ключами
    unsigned long long start_exchange = rdtsc();
    
    mpz_powm(shared_a, public_b, secret_a, params.p);
    mpz_powm(shared_b, public_a, secret_b, params.p);
    
    unsigned long long end_exchange = rdtsc();
    
    // Проверка
    bool success = (mpz_cmp(shared_a, shared_b) == 0);
    
    if (verbose) {
        std::cout << "DH Original: ";
        if (success) {
            std::cout << "Общий ключ успешно выработан!\n";
        } else {
            std::cout << "Ошибка - ключи не совпали!\n";
        }
    }
    
    timing.key_generation = end_keygen - start_keygen;
    timing.key_exchange = end_exchange - start_exchange;
    timing.total = timing.key_generation + timing.key_exchange;
    
    // Очистка
    mpz_clear(secret_a); mpz_clear(secret_b);
    mpz_clear(public_a); mpz_clear(public_b);
    mpz_clear(shared_a); mpz_clear(shared_b);
    gmp_randclear(state);
}

// DH в циклической подгруппе
void diffie_hellman_subgroup(const DHParams& params, TimingResult& timing, bool verbose) {
    mpz_t secret_a, secret_b;
    mpz_t public_a, public_b;
    mpz_t shared_a, shared_b;
    
    mpz_init(secret_a); mpz_init(secret_b);
    mpz_init(public_a); mpz_init(public_b);
    mpz_init(shared_a); mpz_init(shared_b);
    
    gmp_randstate_t state;
    gmp_randinit_default(state);
    
    // Генерация ключей (из Zq, а не Zp!)
    unsigned long long start_keygen = rdtsc();
    
    mpz_urandomm(secret_a, state, params.q);
    mpz_urandomm(secret_b, state, params.q);
    mpz_powm(public_a, params.g, secret_a, params.p);
    mpz_powm(public_b, params.g, secret_b, params.p);
    
    unsigned long long end_keygen = rdtsc();
    
    // Обмен ключами
    unsigned long long start_exchange = rdtsc();
    
    mpz_powm(shared_a, public_b, secret_a, params.p);
    mpz_powm(shared_b, public_a, secret_b, params.p);
    
    unsigned long long end_exchange = rdtsc();
    
    // Проверка
    bool success = (mpz_cmp(shared_a, shared_b) == 0);
    
    if (verbose) {
        std::cout << "DH Subgroup: ";
        if (success) {
            std::cout << "Общий ключ успешно выработан!\n";
        } else {
            std::cout << "Ошибка - ключи не совпали!\n";
        }
    }
    
    timing.key_generation = end_keygen - start_keygen;
    timing.key_exchange = end_exchange - start_exchange;
    timing.total = timing.key_generation + timing.key_exchange;
    
    // Очистка
    mpz_clear(secret_a); mpz_clear(secret_b);
    mpz_clear(public_a); mpz_clear(public_b);
    mpz_clear(shared_a); mpz_clear(shared_b);
    gmp_randclear(state);
}

// Протокол MQV
// Исправленная функция mqv_protocol в crypto_utils.cpp
void mqv_protocol(const DHParams& params, TimingResult& timing, bool verbose) {
    mpz_t a, b; // Долговременные секретные ключи
    mpz_t A, B; // Долговременные открытые ключи
    mpz_t x, y; // Эфемерные секретные ключи
    mpz_t X, Y; // Эфемерные открытые ключи
    mpz_t d, e;
    mpz_t s_a, s_b;
    
    // Инициализация
    mpz_init(a); mpz_init(b);
    mpz_init(A); mpz_init(B);
    mpz_init(x); mpz_init(y);
    mpz_init(X); mpz_init(Y);
    mpz_init(d); mpz_init(e);
    mpz_init(s_a); mpz_init(s_b);
    
    gmp_randstate_t state;
    gmp_randinit_default(state);
    
    unsigned long long start_total = rdtsc();
    unsigned long long start_keygen = rdtsc();
    
    // 1. Долговременные ключи
    mpz_urandomm(a, state, params.q);
    mpz_urandomm(b, state, params.q);
    mpz_powm(A, params.g, a, params.p);
    mpz_powm(B, params.g, b, params.p);
    
    // 2. Эфемерные ключи
    mpz_urandomm(x, state, params.q);
    mpz_urandomm(y, state, params.q);
    mpz_powm(X, params.g, x, params.p);
    mpz_powm(Y, params.g, y, params.p);
    
    unsigned long long end_keygen = rdtsc();
    
    // 3. Вычисление d и e
    int l = mpz_sizeinbase(params.q, 2) / 2;
    mpz_t two_pow_l, mask;
    mpz_init(two_pow_l); mpz_init(mask);
    
    mpz_ui_pow_ui(two_pow_l, 2, l);
    mpz_sub_ui(mask, two_pow_l, 1);
    
    // d = 2^l + (X mod 2^l)
    mpz_and(d, X, mask);
    mpz_add(d, two_pow_l, d);
    
    // e = 2^l + (Y mod 2^l)
    mpz_and(e, Y, mask);
    mpz_add(e, two_pow_l, e);
    
    unsigned long long start_exchange = rdtsc();
    
    // 4. Вычисление общего секрета
    
    // Для пользователя A: s = (Y * B^e)^(x + d*a) mod p
    mpz_t temp1, temp2, temp3, exponent_a, exponent_b;
    mpz_init(temp1); mpz_init(temp2); mpz_init(temp3);
    mpz_init(exponent_a); mpz_init(exponent_b);
    
    // A вычисляет: temp1 = B^e mod p
    mpz_powm(temp1, B, e, params.p);
    // temp2 = Y * temp1 mod p
    mpz_mul(temp2, Y, temp1);
    mpz_mod(temp2, temp2, params.p);
    // exponent_a = x + d*a mod q
    mpz_mul(temp3, d, a);
    mpz_add(exponent_a, x, temp3);
    mpz_mod(exponent_a, exponent_a, params.q);
    // s_a = temp2^exponent_a mod p
    mpz_powm(s_a, temp2, exponent_a, params.p);
    
    // B вычисляет: temp1 = A^d mod p
    mpz_powm(temp1, A, d, params.p);
    // temp2 = X * temp1 mod p
    mpz_mul(temp2, X, temp1);
    mpz_mod(temp2, temp2, params.p);
    // exponent_b = y + e*b mod q
    mpz_mul(temp3, e, b);
    mpz_add(exponent_b, y, temp3);
    mpz_mod(exponent_b, exponent_b, params.q);
    // s_b = temp2^exponent_b mod p
    mpz_powm(s_b, temp2, exponent_b, params.p);
    
    unsigned long long end_exchange = rdtsc();
    unsigned long long end_total = rdtsc();
    
    // Проверка
    bool success = (mpz_cmp(s_a, s_b) == 0);
    
    if (verbose) {
        std::cout << "MQV Protocol: ";
        if (success) {
            std::cout << "Общий секрет успешно выработан!\n";
        } else {
            std::cout << "Ошибка - секреты не совпали!\n";
            // Отладочная информация
            std::cout << "Отладочная информация:\n";
            std::cout << "s_a = "; mpz_out_str(stdout, 10, s_a); std::cout << "\n";
            std::cout << "s_b = "; mpz_out_str(stdout, 10, s_b); std::cout << "\n";
        }
    }
    
    timing.key_generation = end_keygen - start_keygen;
    timing.key_exchange = end_exchange - start_exchange;
    timing.total = end_total - start_total;
    
    // Очистка
    mpz_clear(a); mpz_clear(b);
    mpz_clear(A); mpz_clear(B);
    mpz_clear(x); mpz_clear(y);
    mpz_clear(X); mpz_clear(Y);
    mpz_clear(d); mpz_clear(e);
    mpz_clear(s_a); mpz_clear(s_b);
    mpz_clear(two_pow_l); mpz_clear(mask);
    mpz_clear(temp1); mpz_clear(temp2); mpz_clear(temp3);
    mpz_clear(exponent_a); mpz_clear(exponent_b);
    gmp_randclear(state);
}

// Вывод результатов замера
void print_timing(const std::string& algorithm, const TimingResult& timing) {
    std::cout << "\n=== " << algorithm << " ===\n";
    std::cout << "Генерация ключей: " << timing.key_generation << " циклов\n";
    std::cout << "Обмен ключами:    " << timing.key_exchange << " циклов\n";
    std::cout << "Общее время:      " << timing.total << " циклов\n";
    std::cout << "===================================\n";
}

// Сравнительная таблица
void print_comparison_table(const TimingResult dh_orig, const TimingResult dh_sub, const TimingResult mqv) {
    std::cout << "\n=== СРАВНИТЕЛЬНАЯ ТАБЛИЦА ===\n";
    std::cout << "Алгоритм          | Генерация  | Обмен      | Всего      |\n";
    std::cout << "------------------|------------|------------|------------|\n";
    printf("DH Original       | %10llu | %10llu | %10llu |\n", 
           dh_orig.key_generation, dh_orig.key_exchange, dh_orig.total);
    printf("DH Subgroup       | %10llu | %10llu | %10llu |\n", 
           dh_sub.key_generation, dh_sub.key_exchange, dh_sub.total);
    printf("MQV Protocol      | %10llu | %10llu | %10llu |\n", 
           mqv.key_generation, mqv.key_exchange, mqv.total);
    std::cout << "==============================================\n";
}