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

#define BUFLEN 81

int main() {
    int sockMain, length, msgLength, sendLength;
    struct pollfd fds;
    struct sockaddr_in servAddr, clientAddr;
    char buf[BUFLEN];



    if ((sockMain = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Сервер не может открыть socket для UDP.");
        exit(1);
    }
    bzero((char *) &servAddr, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(0); // Установка порта на 0 для автоматического выбора порта
    fds.fd = sockMain;
    fds.events = POLLIN;

    if (bind(sockMain, (struct sockaddr *)&servAddr, sizeof(servAddr))) {
        perror("Связывание сервера недоступно.");
        exit(1);
    }

    length = sizeof(servAddr);
    if (getsockname(sockMain, (struct sockaddr *)&servAddr, (socklen_t *)&length)) {
        perror("Вызов getsockname неудачен.");
        exit(1);
    }
    printf("SERVER: номер порта - %d\n", ntohs(servAddr.sin_port));
    char otvet[] = {"ответ дада"};
    for (;;) {
        length = sizeof(clientAddr);
        bzero(buf, BUFLEN);

        if (0 ==poll(&fds, 1, 20000))
        {   
            perror("Время вышло");
            exit(1);
        }
        if ((msgLength = recvfrom(sockMain, buf, BUFLEN, 0, (struct sockaddr *)&clientAddr, (socklen_t *)&length)) < 0) {
            perror("Плохой socket клиента.");
            exit(1);
        }
        if((sendLength = (sendto(sockMain, otvet, strlen(otvet), 0, (struct sockaddr *)&clientAddr, (socklen_t)length))) < 0){
            perror("Плохой socket клиента.");
            exit(1);
        }
        printf("SERVER: IP адрес клиента: %s\n", inet_ntoa(clientAddr.sin_addr));
        printf("SERVER: PORT клиента: %d\n", ntohs(clientAddr.sin_port));
        printf("SERVER: Длина сообщения - %d\n", msgLength);
        printf("SERVER: Сообщение: %s\n\n", buf);
    }
    printf("тест");

    close(sockMain);
    return 0;
}