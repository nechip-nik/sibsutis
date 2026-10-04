#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

static volatile sig_atomic_t running = 1;
static int sock = -1;

void sigint_handler(int signo) {
    (void)signo;
    running = 0;
    if (sock != -1) {
        shutdown(sock, SHUT_RDWR); 
    }
}

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int n, result;

    if (argc != 4) {
        fprintf(stderr, "Использование: %s <IP-адрес сервера> <порт> <число N>\n", argv[0]);
        fprintf(stderr, "Пример: %s 127.0.0.1 8080 3\n", argv[0]);
        exit(1);
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);
    n = atoi(argv[3]);

    if (n <= 0) {
        fprintf(stderr, "Число N должно быть положительным\n");
        exit(1);
    }

    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("[Клиент] Ошибка sigaction");
        exit(1);
    }

    printf("[Клиент] Запуск: сервер=%s, порт=%d, N=%d\n", server_ip, port, n);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("[Клиент] Ошибка socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("[Клиент] Ошибка inet_pton");
        close(sock);
        exit(1);
    }

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("[Клиент] Ошибка connect");
        close(sock);
        exit(1);
    }
    printf("[Клиент] Подключен к серверу %s:%d\n", server_ip, port);

    while (running) {
        snprintf(buffer, BUFFER_SIZE, "%d", n);
        ssize_t sent = send(sock, buffer, strlen(buffer), 0);
        if (sent == -1) {
            if (errno == EINTR) continue;  
            perror("[Клиент] Ошибка send");
            break;
        }
        printf("[Клиент] Отправлено число: %d\n", n);

        ssize_t bytes_read = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                printf("[Клиент] Сервер закрыл соединение\n");
            } else if (errno != EINTR) {
                perror("[Клиент] Ошибка recv");
            }
            break;
        }
        buffer[bytes_read] = '\0';
        result = atoi(buffer);
        printf("[Клиент] Получен результат: %d (N=%d * 2)\n", result, n);
        for (int i = 0; i < n && running; ++i) {
            sleep(1);
        }
    }

    printf("[Клиент] Завершение работы, закрываю соединение...\n");
    if (sock != -1) {
        close(sock);
        sock = -1;
    }

    return 0;
}