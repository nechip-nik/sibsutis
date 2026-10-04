#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

#define BACKLOG 10
#define BUFFER_SIZE 1024

static volatile sig_atomic_t running = 1;
static int server_sock = -1;


void sigchld_handler(int signo) {
    (void)signo;
    pid_t pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        char buf[64];
        int len = snprintf(buf, sizeof(buf), "[Сервер] Дочерний PID=%d завершился\n", pid);
        write(STDOUT_FILENO, buf, len);
    }
}

void sigint_handler(int signo) {
    (void)signo;
    running = 0;
    if (server_sock != -1) {
        close(server_sock);
        server_sock = -1;
    }
}

void handle_client(int client_sock, struct sockaddr_in client_addr) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    pid_t pid = getpid();
    char client_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(client_addr.sin_port);

    printf("[Сервер] Дочерний PID=%d обслуживает клиента %s:%d\n",
           pid, client_ip, client_port);

    while (1) {
        bytes_read = recv(client_sock, buffer, BUFFER_SIZE - 1, 0);

        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                printf("[Сервер] PID=%d: клиент %s:%d отключился\n",
                       pid, client_ip, client_port);
            } else if (errno != EINTR) {
                perror("[Сервер] Ошибка recv");
            }
            break;
        }

        buffer[bytes_read] = '\0';
        int n = atoi(buffer);

        printf("[Сервер] PID=%d: получено число %d от %s:%d\n",
               pid, n, client_ip, client_port);

        int result = n * 2;
        snprintf(buffer, BUFFER_SIZE, "%d", result);

        if (send(client_sock, buffer, strlen(buffer), 0) == -1) {
            perror("[Сервер] Ошибка send");
            break;
        }

        printf("[Сервер] PID=%d: отправлен результат %d клиенту %s:%d\n",
               pid, result, client_ip, client_port);
    }

    close(client_sock);
    printf("[Сервер] PID=%d: завершаю обслуживание клиента\n", pid);
    exit(0);
}

int main() {
    int client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pid_t pid;

    // SIGCHLD — сбор зомби
    struct sigaction sa_chld;
    sa_chld.sa_handler = sigchld_handler;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa_chld, NULL) == -1) {
        perror("[Сервер] Ошибка sigaction(SIGCHLD)");
        exit(1);
    }

    struct sigaction sa_int;
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    if (sigaction(SIGINT, &sa_int, NULL) == -1) {
        perror("[Сервер] Ошибка sigaction(SIGINT)");
        exit(1);
    }

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("[Сервер] Ошибка socket");
        exit(1);
    }

    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = 0;

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("[Сервер] Ошибка bind");
        close(server_sock);
        exit(1);
    }

    socklen_t addr_len = sizeof(server_addr);
    if (getsockname(server_sock, (struct sockaddr*)&server_addr, &addr_len) == -1) {
        perror("[Сервер] Ошибка getsockname");
        close(server_sock);
        exit(1);
    }
    printf("[Сервер] Сервер запущен на порту: %d\n", ntohs(server_addr.sin_port));

    if (listen(server_sock, BACKLOG) == -1) {
        perror("[Сервер] Ошибка listen");
        close(server_sock);
        exit(1);
    }

    while (running) {
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
        if (client_sock == -1) {
            if (errno == EINTR) continue;
            if (!running) break;             
            perror("[Сервер] Ошибка accept");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
        int client_port = ntohs(client_addr.sin_port);
        printf("[Сервер] Новое подключение от %s:%d\n", client_ip, client_port);

        pid = fork();
        if (pid == -1) {
            perror("[Сервер] Ошибка fork");
            close(client_sock);
            continue;
        }

        if (pid == 0) {
            close(server_sock);
            server_sock = -1;
            signal(SIGINT, SIG_DFL);
            handle_client(client_sock, client_addr);
        } else {
            close(client_sock);
            printf("[Сервер] Создан дочерний PID=%d для клиента %s:%d\n",
                   pid, client_ip, client_port);
        }
    }

    printf("[Сервер] Завершение работы сервера\n");
    if (server_sock != -1) close(server_sock);
    return 0;
}