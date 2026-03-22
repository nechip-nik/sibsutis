#include <arpa/inet.h>
#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080

int main() {
  int sock = 0;
  struct sockaddr_in serv_addr;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\nConnection Failed \n");
    return -1;
  }

  char login[50];
  printf("Enter login: ");
  scanf("%s", login);
  write(sock, login, strlen(login));

  mpz_t n, v, p, q, s, r, x, y, tmp;
  mpz_inits(n, v, p, q, s, r, x, y, tmp, NULL);

  // Генерация закрытого ключа
  mpz_urandomb(p, gmp_randstate_t state, 512);
  mpz_urandomb(q, gmp_randstate_t state, 512);
  mpz_mul(n, p, q);

  mpz_urandomb(s, gmp_randstate_t state, 512);
  mpz_powm_ui(v, s, 2, n);

  // Генерация случайного числа r
  mpz_urandomb(r, gmp_randstate_t state, 512);
  mpz_powm_ui(x, r, 2, n);

  // Отправка x на сервер
  char x_str[1024];
  mpz_get_str(x_str, 10, x);
  write(sock, x_str, strlen(x_str));

  int b;
  read(sock, &b, sizeof(b));

  if (b == 0) {
    char r_str[1024];
    mpz_get_str(r_str, 10, r);
    write(sock, r_str, strlen(r_str));
  } else {
    mpz_mul(y, r, s);
    mpz_mod(y, y, n);
    char y_str[1024];
    mpz_get_str(y_str, 10, y);
    write(sock, y_str, strlen(y_str));
  }

  char response[1024];
  read(sock, response, 1024);
  printf("Server response: %s\n", response);

  mpz_clears(n, v, p, q, s, r, x, y, tmp, NULL);

  close(sock);
  return 0;
}