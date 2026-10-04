#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

typedef struct {
    struct sockaddr_in addr;
    socklen_t addr_len;
    int last_seq;
    int packets_received;
    int packets_lost;
} client_info_t;

client_info_t clients[MAX_CLIENTS];
int client_count = 0;
int total_packets_received = 0;
int total_packets_lost = 0;

int should_drop_packet(int losses) {
    return (rand() % 100) < losses;
}

int find_client(struct sockaddr_in *addr) {
    for (int i = 0; i < client_count; i++) {
        if (clients[i].addr.sin_addr.s_addr == addr->sin_addr.s_addr &&
            clients[i].addr.sin_port == addr->sin_port) {
            return i;
        }
    }
    return -1;
}

int add_client(struct sockaddr_in *addr, socklen_t addr_len) {
    if (client_count < MAX_CLIENTS) {
        clients[client_count].addr = *addr;
        clients[client_count].addr_len = addr_len;
        clients[client_count].last_seq = -1;
        clients[client_count].packets_received = 0;
        clients[client_count].packets_lost = 0;  
        return client_count++;
    }
    return -1;
}

void signal_handler(int sig) {
    printf("\n\n=== СТАТИСТИКА РАБОТЫ СЕРВЕРА ===\n");
    printf("Всего получено пакетов: %d\n", total_packets_received);
    printf("Всего потеряно пакетов: %d\n", total_packets_lost);
    printf("Процент потерь: %.2f%%\n", 
           total_packets_received + total_packets_lost > 0 ? 
           (float)total_packets_lost / (total_packets_received + total_packets_lost) * 100 : 0);
    
    printf("\nИнформация по клиентам:\n");
    for (int i = 0; i < client_count; i++) {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clients[i].addr.sin_addr, ip, INET_ADDRSTRLEN);
        int total = clients[i].packets_received + clients[i].packets_lost;
        printf("Клиент %s:%d - получено: %d, потеряно: %d\n", 
               ip, ntohs(clients[i].addr.sin_port),
               clients[i].packets_received, 
               clients[i].packets_lost);
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <Процент потерь>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int losses = atoi(argv[1]);

    if (losses < 0 || losses > 100)
    {
        fprintf(stderr, "Процент должен быть от 0 до 100\n");
        exit(EXIT_FAILURE);
    }
    
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    char ack_buffer[BUFFER_SIZE];
    
    srand(time(NULL));

    signal(SIGINT, signal_handler);
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = 0;
    
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    socklen_t len = sizeof(server_addr);
    if (getsockname(sockfd, (struct sockaddr *)&server_addr, &len) < 0) {
        perror("getsockname failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Сервер запущен на порту: %d\n", ntohs(server_addr.sin_port));
    printf("Вероятность потери пакетов: %d%%\n", losses);
    printf("------------------------------------------------\n");
    
    while (1) {
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, 
                        (struct sockaddr *)&client_addr, &client_addr_len);
        if (n < 0) {
            perror("recvfrom failed");
            continue;
        }
        buffer[n] = '\0';
        
        int seq_num;
        char message[BUFFER_SIZE];
        if (sscanf(buffer, "SEQ:%d:%s", &seq_num, message) != 2) {
            printf("Получено некорректное сообщение: %s\n", buffer);
            continue;
        }
        
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        
        int client_idx = find_client(&client_addr);
        if (client_idx == -1) {
            client_idx = add_client(&client_addr, client_addr_len);
            if (client_idx == -1) {
                printf("Достигнуто максимальное количество клиентов!\n");
                continue;
            }
        }
        
        if (should_drop_packet(losses)) {
            total_packets_lost++;
            clients[client_idx].packets_lost++;
            
            printf("[LOST] Пакет SEQ:%d от %s:%d (имитация потери)\n", 
                   seq_num, client_ip, ntohs(client_addr.sin_port));
            continue;  
        }
        
        if (clients[client_idx].last_seq != -1 && seq_num != clients[client_idx].last_seq + 1) {
            printf("[WARNING] Нарушение последовательности от клиента %s:%d. "
                   "Ожидалось: %d, получено: %d\n",
                   client_ip, ntohs(client_addr.sin_port),
                   clients[client_idx].last_seq + 1, seq_num);
        }
        
        clients[client_idx].last_seq = seq_num;
        clients[client_idx].packets_received++;
        total_packets_received++;
        
        printf("[RECV] SEQ:%d от %s:%d: %s\n", 
               seq_num, client_ip, ntohs(client_addr.sin_port), message);
        
        snprintf(ack_buffer, BUFFER_SIZE, "ACK:%d", seq_num);
        if (sendto(sockfd, ack_buffer, strlen(ack_buffer), 0,
                  (const struct sockaddr *)&client_addr, client_addr_len) < 0) {
            perror("sendto failed");
        } else {
            printf("[SEND] Подтверждение ACK:%d отправлено клиенту %s:%d\n", 
                   seq_num, client_ip, ntohs(client_addr.sin_port));
        }
    }
    
    close(sockfd);
    return 0;
}