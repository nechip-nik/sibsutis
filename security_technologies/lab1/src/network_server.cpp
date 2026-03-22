#include "crypto_utils.h"
#include "rdtsc.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    std::cout << "=== СЕРВЕР (ПОЛЬЗОВАТЕЛЬ A) ===\n\n";
    
    // Загрузка параметров
    DHParams params;
    params.loadFromFile("params_mult.txt");
    
    std::cout << "Параметры загружены.\n";
    
    // Создание сокета
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Ошибка создания сокета\n";
        return 1;
    }
    
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    
    // Привязка сокета
    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Ошибка привязки сокета\n";
        return 1;
    }
    
    // Ожидание подключения
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Ошибка listen\n";
        return 1;
    }
    
    std::cout << "Сервер ожидает подключения на порту 8080...\n";
    
    sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_socket = accept(server_fd, (sockaddr*)&client_addr, &addr_len);
    
    if (client_socket < 0) {
        std::cerr << "Ошибка accept\n";
        return 1;
    }
    
    std::cout << "Клиент подключен!\n\n";
    
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
    
    // Отправка открытого ключа клиенту
    char pub_key_str[1024];
    mpz_get_str(pub_key_str, 10, public_key);
    send(client_socket, pub_key_str, strlen(pub_key_str) + 1, 0);
    
    std::cout << "Открытый ключ отправлен клиенту.\n";
    
    // Получение открытого ключа от клиента
    char client_pub_key_str[1024];
    recv(client_socket, client_pub_key_str, sizeof(client_pub_key_str), 0);
    
    mpz_t client_pub_key;
    mpz_init(client_pub_key);
    mpz_set_str(client_pub_key, client_pub_key_str, 10);
    
    // Вычисление общего ключа
    std::cout << "Вычисляю общий ключ...\n";
    start = rdtsc();
    
    mpz_powm(shared_key, client_pub_key, secret, params.p);
    
    end = rdtsc();
    std::cout << "Время вычисления общего ключа: " << (end - start) << " циклов\n\n";
    
    // Вывод результатов
    std::cout << "Секретный ключ сервера: ";
    mpz_out_str(stdout, 10, secret);
    std::cout << "\n";
    
    std::cout << "Открытый ключ сервера: ";
    mpz_out_str(stdout, 10, public_key);
    std::cout << "\n";
    
    std::cout << "Общий ключ: ";
    mpz_out_str(stdout, 10, shared_key);
    std::cout << "\n";
    
    // Закрытие соединений
    close(client_socket);
    close(server_fd);
    
    // Очистка
    mpz_clear(secret);
    mpz_clear(public_key);
    mpz_clear(shared_key);
    mpz_clear(client_pub_key);
    gmp_randclear(state);
    
    std::cout << "\nСервер завершил работу.\n";
    
    return 0;
}