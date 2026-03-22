#include "crypto_utils.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "rdtsc.h"

int main() {
    std::cout << "=== КЛИЕНТ (ПОЛЬЗОВАТЕЛЬ B) ===\n\n";
    
    // Загрузка параметров
    DHParams params;
    params.loadFromFile("params_mult.txt");
    
    std::cout << "Параметры загружены.\n";
    
    // Создание сокета
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Ошибка создания сокета\n";
        return 1;
    }
    
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
    
    std::cout << "Подключаюсь к серверу...\n";
    
    if (connect(sock, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Ошибка подключения\n";
        return 1;
    }
    
    std::cout << "Подключение установлено!\n\n";
    
    // Генерация ключей
    mpz_t secret, public_key, shared_key;
    mpz_init(secret); 
    mpz_init(public_key); 
    mpz_init(shared_key);
    
    gmp_randstate_t state;
    gmp_randinit_default(state);
    
    std::cout << "Генерирую секретный и открытый ключи...\n";
    unsigned long long start = rdtsc();
    
    mpz_urandomm(secret, state, params.p);
    mpz_powm(public_key, params.g, secret, params.p);
    
    unsigned long long end = rdtsc();
    std::cout << "Время генерации ключей: " << (end - start) << " циклов\n";
    
    // Получение открытого ключа от сервера
    char server_pub_key_str[1024];
    recv(sock, server_pub_key_str, sizeof(server_pub_key_str), 0);
    
    mpz_t server_pub_key;
    mpz_init(server_pub_key);
    mpz_set_str(server_pub_key, server_pub_key_str, 10);
    
    // Отправка своего открытого ключа
    char pub_key_str[1024];
    mpz_get_str(pub_key_str, 10, public_key);
    send(sock, pub_key_str, strlen(pub_key_str) + 1, 0);
    
    std::cout << "Открытый ключ отправлен серверу.\n";
    
    // Вычисление общего ключа
    std::cout << "Вычисляю общий ключ...\n";
    start = rdtsc();
    
    mpz_powm(shared_key, server_pub_key, secret, params.p);
    
    end = rdtsc();
    std::cout << "Время вычисления общего ключа: " << (end - start) << " циклов\n\n";
    
    // Вывод результатов
    std::cout << "Секретный ключ клиента: ";
    mpz_out_str(stdout, 10, secret);
    std::cout << "\n";
    
    std::cout << "Открытый ключ клиента: ";
    mpz_out_str(stdout, 10, public_key);
    std::cout << "\n";
    
    std::cout << "Общий ключ: ";
    mpz_out_str(stdout, 10, shared_key);
    std::cout << "\n\n";
    
    std::cout << "Проверка: общие ключи у сервера и клиента должны совпадать!\n";
    
    // Закрытие соединения
    close(sock);
    
    // Очистка
    mpz_clear(secret);
    mpz_clear(public_key);
    mpz_clear(shared_key);
    mpz_clear(server_pub_key);
    gmp_randclear(state);
    
    std::cout << "\nКлиент завершил работу.\n";
    
    return 0;
}