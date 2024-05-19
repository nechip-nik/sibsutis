#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER "127.0.0.1" // Замените на IP-адрес сервера, если он не локальный
#define BUFLEN 512
#define PORT 8888

int sock;
struct sockaddr_in server;
char message[BUFLEN];

void *send_msg(void *arg) {
  while (1) {
    if (fgets(message, BUFLEN, stdin) == NULL) {
      printf("Error reading input.\n");
      continue;
    }

    // Удаление символа новой строки
    size_t len = strlen(message);
    if (len > 0 && message[len - 1] == '\n') {
      message[len - 1] = '\0'; // Удаление символа новой строки
    }

    // Проверка на пустое сообщение
    if (strlen(message) == 0 || strspn(message, " ") == strlen(message)) {
      printf("Empty message, skipping.\n");
      continue;
    }
    if (!(strcmp(message, "/help"))) {
      printf(
          "\n/login - команда для регистрации или входа.\n/chat - вывод "
          "чата.\n/users - вывод ников всех пользователей.\n/exit - выход.\n");
      continue;
    }
    if (strcmp(message, "/exit") == 0) {
      sendto(sock, "/exit", strlen(message), 0, (struct sockaddr *)&server,
             sizeof(server));
      exit(0); // Завершаем программу с кодом возврата 0
    }

    // Отправка сообщения на сервер
    if (sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server,
               sizeof(server)) == -1) {
      perror("sendto()");
      break;
    }

    // Обнуление буфера после отправки
    memset(message, 0, sizeof(message));
  }
  pthread_exit(NULL);
}

void *receive_msg(void *arg) {
  struct sockaddr_in si_other;
  socklen_t slen = sizeof(si_other);
  char buf[BUFLEN];

  while (1) {
    // Получение сообщения от сервера
    if (recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *)&si_other, &slen) ==
        -1) {
      perror("recvfrom()");
      break;
    }
    if (strcmp(buf, "/login") == 0) {
      memset(buf, 0, sizeof(buf));
      recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *)&si_other, &slen);
      printf("\n%s :", buf);
    } else {
      printf("%s\n", buf); // Вывод с новой строки для разделения сообщений
    }
    memset(buf, 0, sizeof(buf));
  }
  pthread_exit(NULL);
}

int main(void) {
  printf("Войдите в аккаунт или зарегистрируйтесь используя \"/login\" <login> "
         "<password>\n \"/help\" - список команд.\n");
  // Создание сокета
  if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    perror("socket");
    return EXIT_FAILURE;
  }

  // Заполнение информации о сервере
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  if (inet_aton(SERVER, &server.sin_addr) == 0) {
    fprintf(stderr, "inet_aton() failed\n");
    close(sock);
    return EXIT_FAILURE;
  }

  // Создание потоков для отправки и получения сообщений
  pthread_t send_thread, receive_thread;
  if (pthread_create(&send_thread, NULL, send_msg, NULL) != 0 ||
      pthread_create(&receive_thread, NULL, receive_msg, NULL) != 0) {
    perror("pthread_create");
    close(sock);
    return EXIT_FAILURE;
  }

  // Ожидание завершения потоков
  pthread_join(send_thread, NULL);
  pthread_join(receive_thread, NULL);

  close(sock);
  return 0;
}