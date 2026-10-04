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
#define TIMEOUT_SEC 2
#define MAX_RETRIES 3

int packets_sent = 0;
int packets_acknowledged = 0;
int packets_retransmitted = 0;
int running = 1;

void signal_handler(int sig) {
    running = 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Использование: %s <IP-адрес сервера> <порт>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char ack_buffer[BUFFER_SIZE];
    struct timeval tv;
    int seq_num = 0;
    
    signal(SIGINT, signal_handler);
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
    
    tv.tv_sec = TIMEOUT_SEC;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Клиент запущен. Подключение к серверу %s:%s\n", argv[1], argv[2]);
    printf("------------------------------------------------\n");
    
    while (running) {
        char message[BUFFER_SIZE];
        snprintf(message, BUFFER_SIZE, "Message_%d", seq_num);
        
        char send_buffer[BUFFER_SIZE];
        snprintf(send_buffer, BUFFER_SIZE, "SEQ:%d:%s", seq_num, message);
        
        int retries = 0;
        int ack_received = 0;
        
        while (retries < MAX_RETRIES && !ack_received && running) {
            // Отправка данных
            if (sendto(sockfd, send_buffer, strlen(send_buffer), 0,
                      (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                perror("sendto failed");
                break;
            }
            
            packets_sent++;
            if (retries > 0) {
                packets_retransmitted++;
                printf("[RETRY] Повторная отправка SEQ:%d (попытка %d)\n", 
                       seq_num, retries + 1);
            } else {
                printf("[SEND] Отправлено SEQ:%d: %s\n", seq_num, message);
            }
            
            // Ожидание подтверждения
            socklen_t addr_len = sizeof(server_addr);
            int n = recvfrom(sockfd, ack_buffer, BUFFER_SIZE, 0,
                           (struct sockaddr *)&server_addr, &addr_len);
            
            if (n > 0) {
                ack_buffer[n] = '\0';
                int ack_seq;
                if (sscanf(ack_buffer, "ACK:%d", &ack_seq) == 1 && ack_seq == seq_num) {
                    printf("[ACK] Получено подтверждение ACK:%d\n", seq_num);
                    packets_acknowledged++;
                    ack_received = 1;
                } else {
                    printf("[WARNING] Получено некорректное подтверждение: %s\n", ack_buffer);
                }
            } else {
                printf("[TIMEOUT] Таймаут ожидания ACK:%d\n", seq_num);
            }
            
            retries++;
        }
        
        if (!ack_received) {
            printf("[ERROR] Не удалось получить подтверждение для SEQ:%d\n", seq_num);
        }
        
        seq_num++;
        
        sleep(1);
    }
    
    printf("\n=== СТАТИСТИКА РАБОТЫ КЛИЕНТА ===\n");
    printf("Отправлено пакетов: %d\n", packets_sent);
    printf("Подтверждено пакетов: %d\n", packets_acknowledged);
    printf("Повторных отправок: %d\n", packets_retransmitted);
    printf("Потеряно пакетов: %d\n", packets_sent - packets_acknowledged);
    printf("Процент потерь: %.2f%%\n", 
           packets_sent > 0 ? (float)(packets_sent - packets_acknowledged) / packets_sent * 100 : 0);
    
    close(sockfd);
    return 0;
}