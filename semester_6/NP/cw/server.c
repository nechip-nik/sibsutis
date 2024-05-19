#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

#define BUFLEN 512
#define PORT 8888
#define MAX_CLIENTS 1

// Структура для хранения адресов клиентов
struct client_info {
    struct sockaddr_in addr;
    int active;
    char login[32];
    char password[32];
    int authenticated;

};

// Функция для проверки логина и пароля
int check_credentials(const char *login, const char *password, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка при открытии файла с логинами и паролями");
        return 0;
    }

    char stored_login[32], stored_password[32];
    while (fscanf(file, "%s %s", stored_login, stored_password) == 2) {
        if (strcmp(stored_login, login) == 0 && strcmp(stored_password, password) == 0) {
            fclose(file);
            return 1;
        }
        else if (strcmp(stored_login, login) == 0)
        {
            return 2;
        }
        
    }

    fclose(file);
    return 0;
}
void write_to_file(const char *filename, const char *message, char *login) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return;
    }
    fprintf(file, "%s: %s\n", login,message);
    fclose(file);
}

int main() {
    int sockMain, msgLength, sendLength;
    struct pollfd fds;
    struct sockaddr_in servAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buf[BUFLEN];
    struct client_info clients[MAX_CLIENTS] = {0}; // Массив для хранения адресов клиентов
    char *strings[10];

    // Выделение памяти для каждой строки
    for (int i = 0; i < 10; i++) {
        strings[i] = malloc(32 * sizeof(char)); // Выделяем 20 символов для каждой строки
        if (strings[i] == NULL) {
            printf("Ошибка выделения памяти для строки %d\n", i);
            return 1;
        }
    }

    if ((sockMain = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Сервер не может открыть socket для UDP.");
        exit(1);
    }
    bzero((char *) &servAddr, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(PORT);
    fds.fd = sockMain;
    fds.events = POLLIN;

    if (bind(sockMain, (struct sockaddr *)&servAddr, sizeof(servAddr))) {
        perror("Связывание сервера недоступно.");
        exit(1);
    }

    if (getsockname(sockMain, (struct sockaddr *)&servAddr, &clientAddrLen)) {
        perror("Вызов getsockname неудачен.");
        exit(1);
    }

    while (1) {
        // Ожидание входящих данных
        if (poll(&fds, 1, -1) < 0) {
            perror("Ошибка опроса сокета");
            exit(1);
        }

        // Проверка наличия входящих данных
        if (fds.revents & POLLIN) {
            bzero(buf, BUFLEN);
            // Получение сообщения от клиента
            if ((msgLength = recvfrom(sockMain, buf, BUFLEN, 0, (struct sockaddr *)&clientAddr, &clientAddrLen)) < 0) {
                perror("Ошибка при получении сообщения от клиента");
                exit(1);
            }

            // Проверка, есть ли уже такой клиент в списке
            int clientIndex = -1;
            for (int i = 0; i < MAX_CLIENTS; ++i) {
                if (clients[i].active && memcmp(&clients[i].addr, &clientAddr, sizeof(clientAddr)) == 0) {
                    clientIndex = i;
                    break;
                }
            }
            

            // Если клиент новый, добавляем его в список
            if (clientIndex == -1) {
                for (int i = 0; i < MAX_CLIENTS; ++i) {
                    if (!clients[i].active) {
                        clients[i].addr = clientAddr;
                        clients[i].active = 1;
                        clientIndex = i;
                        break;
                    }
                }
            }

            // Если список клиентов переполнен, игнорируем новое сообщение
            if (clientIndex == -1) {
                printf("SERVER: Список клиентов переполнен, игнорируем сообщение от %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                sendto(sockMain, "Ошибка", 32, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
                continue;
            }
            if (strncmp(buf, "/login ", 7) == 0) {
                char login[32] = {0}, password[32] = {0};
                sscanf(buf, "/login %s %s", login, password);
                for (int i = 0; i < MAX_CLIENTS; i++)
                {
                    if (strcmp(strings[i], login) == 0)
                    {
                        memset(login, 0, sizeof(login));
                        memset(password, 0, sizeof(login));
                        break;
                    }
                    
                }
                
                if ((strlen(login) == 0) && (strlen(password) == 0)) {
                    sendto(sockMain, "Ошибка в пароле.", 32, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
                    continue;
                }
                
                if (check_credentials(login, password, "credentials.txt") == 1) {
                    strcpy(clients[clientIndex].login, login);
                    strcpy(clients[clientIndex].password, password);
                    clients[clientIndex].authenticated = 1;
                    strcpy(strings[clientIndex], login);
                    printf("SERVER: Клиент %s:%d успешно авторизован\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                    sendto(sockMain, "Вы зашли", 32, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
                    continue;
                } 
                else if (check_credentials(login, password, "credentials.txt") == 0){
                    strcpy(clients[clientIndex].login, login);
                    strcpy(clients[clientIndex].password, password);
                    clients[clientIndex].authenticated = 1;
                    strcpy(strings[clientIndex], login);
                    FILE *file = fopen("credentials.txt", "a");
                    fprintf(file, "\n%s %s", login, password);
                    fclose(file);
                    printf("SERVER: Клиент %s:%d успеttшно авторизован\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                    sendto(sockMain, "Вы зашли", 32, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
                    continue;
                }
                else if (check_credentials(login, password, "credentials.txt") == 2)
                {
                    sendto(sockMain, "Неверный пароль.", 32, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
                    printf("SERVER: Клиент %s:%d неверный пароль\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                }
                
                else {
                    printf("SERVER: Ошибка авторизации для клиента %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                    sendto(sockMain, "Ошибка", 32, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
                    continue;
                }
            }
            // Проверка, авторизован ли клиент
            if (!clients[clientIndex].active || clients[clientIndex].login[0] == '\0' || clients[clientIndex].password[0] == '\0' || !clients[clientIndex].authenticated) {
                printf("SERVER: Клиент %s:%d не авторизован \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                sendto(sockMain, "Авторизуйтесь", 32, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
                    
                    clients[clientIndex].active = 0;
                    memset(strings[clientIndex], 0, sizeof(strings));
                        
                    

                continue;
            }
            if (strcmp(buf, "/chat") == 0) {
                char buffer[1024];
                FILE *file = fopen("chat.txt", "r");
                while (fgets(buffer, sizeof(buffer), file) != NULL) {
                if ((sendLength = sendto(sockMain, buffer, 34, 0, (struct sockaddr *)&clientAddr, clientAddrLen)) < 0) {
                        perror("Ошибка при отправке сообщения клиенту");
                        exit(1);
                }
                }
                fclose(file);
                continue;
            }
            else{
                write_to_file("chat.txt", buf, strings[clientIndex]);
            }
            if (strcmp(buf, "/user") == 0){
                for (int i = 0; i < MAX_CLIENTS; ++i){
                    if ((strlen(strings[i]) == 0))
                    {
                        continue;
                    }
                    
                    else if ((sendto(sockMain, strings[i], 32, 0, (struct sockaddr *)&clientAddr, clientAddrLen)) < 0) {
                        perror("Ошибка при отправке сообщения клиенту");
                        exit(1);
                    }
                    
                    
                }
                continue;
            }
            if (strcmp(buf, "/exit") == 0){
                memset(strings[clientIndex], 0, sizeof(strings));
                clients[clientIndex].active = 0;
                printf("SERVER: Клиент %s:%d удален \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                continue;
            }
            
            // Отправка сообщения всем клиентам, кроме только что получившего сообщение
            for (int i = 0; i < MAX_CLIENTS; ++i) {
                if (clients[i].active && i != clientIndex) {
                    if ((sendto(sockMain, "/login", 32, 0, (struct sockaddr *)&clients[i].addr, clientAddrLen)) < 0) {
                        perror("Ошибка при отправке сообщения клиенту");
                        exit(1);
                    }
                    if ((sendto(sockMain, strings[clientIndex], 32, 0, (struct sockaddr *)&clients[i].addr, clientAddrLen)) < 0) {
                        perror("Ошибка при отправке сообщения клиенту");
                        exit(1);
                    }
                    if ((sendLength = sendto(sockMain, buf, msgLength, 0, (struct sockaddr *)&clients[i].addr, clientAddrLen)) < 0) {
                        perror("Ошибка при отправке сообщения клиенту");
                        exit(1);
                    }
                }
            }
        }
    }
    for (int i = 0; i < 10; i++) {
        free(strings[i]);
    }

    close(sockMain);
    return 0;
}