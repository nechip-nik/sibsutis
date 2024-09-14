#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER "127.0.0.1"
#define BUFLEN 512
#define PORT 8888

void die(char *s) {
    perror(s);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        exit(1);
    }

    int number = atoi(argv[1]);
    if (number < 1 || number > 10) {
        fprintf(stderr, "Number must be between 1 and 10\n");
        exit(1);
    }

    struct sockaddr_in si_other;
    int s, i, slen = sizeof(si_other);
    char message[BUFLEN];

    // Создание сокета
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        die("socket");
    }

    // Заполнение структуры адреса
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(SERVER, &si_other.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    // Отправка сообщений с задержкой
    for (i = 1; i ; i++) {
        sprintf(message, "%d", number);

        // Отправка сообщения
        if (sendto(s, message, strlen(message), 0, (struct sockaddr *)&si_other, slen) == -1) {
            die("sendto()");
        }

        // Задержка перед отправкой следующего сообщения
        sleep(number);
    }

    close(s);
    return 0;
}