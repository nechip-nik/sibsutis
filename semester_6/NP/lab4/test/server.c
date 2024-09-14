#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

#define BUFLEN 512
#define PORT 8888

void die(char *s) {
    perror(s);
    exit(1);
}

int main(void) {
    struct sockaddr_in si_me, si_other;
    int s, i, slen = sizeof(si_other);
    char buf[BUFLEN];
    fd_set readfds;
    struct timeval timeout;

    // Создание сокета
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        die("socket");
    }

    // Заполнение структуры адреса
    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    // Привязка сокета к адресу
    if (bind(s, (struct sockaddr *)&si_me, sizeof(si_me)) == -1) {
        die("bind");
    }

    // Цикл обработки сообщений
    while (1) {
        // Очистка множества дескрипторов
        FD_ZERO(&readfds);
        // Добавление сокета в множество
        FD_SET(s, &readfds);

        // Установка таймаута для select
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        // Ожидание событий на сокете
        if (select(s + 1, &readfds, NULL, NULL, &timeout) == -1) {
            die("select");
        }

        // Проверка наличия событий на сокете
        if (FD_ISSET(s, &readfds)) {
            // Получение данных
            if ((recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_other, &slen)) == -1) {
                die("recvfrom()");
            }

            printf("Получено пакет от %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
            printf("Сообщение: %s\n", buf);

            // Отправка ответа
            if (sendto(s, "Получено\n", 9, 0, (struct sockaddr *)&si_other, slen) == -1) {
                die("sendto()");
            }
        }
    }

    close(s);
    return 0;
}