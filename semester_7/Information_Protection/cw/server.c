#include <arpa/inet.h>
#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENTS 10

typedef struct {
  char login[50];
  mpz_t n;
  mpz_t v;
} User;

User users[MAX_CLIENTS];
int user_count = 0;

void load_users() {
  FILE *file = fopen("users.txt", "r");
  if (!file) {
    perror("Failed to open users file");
    exit(1);
  }

  while (user_count < MAX_CLIENTS) {
    User user;
    char n_str[1024], v_str[1024];
    if (fscanf(file, "%s %s %s", user.login, n_str, v_str) != 3)
      break;
    mpz_init_set_str(user.n, n_str, 10);
    mpz_init_set_str(user.v, v_str, 10);
    users[user_count++] = user;
  }

  fclose(file);
}

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  load_users();

  while (1) {
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    char login[50];
    read(new_socket, login, 50);

    User *user = NULL;
    for (int i = 0; i < user_count; i++) {
      if (strcmp(users[i].login, login) == 0) {
        user = &users[i];
        break;
      }
    }

    if (!user) {
      write(new_socket, "Invalid login", 14);
      close(new_socket);
      continue;
    }

    mpz_t x, y, r, tmp;
    mpz_inits(x, y, r, tmp, NULL);

    char x_str[1024];
    read(new_socket, x_str, 1024);
    mpz_set_str(x, x_str, 10);

    int b = rand() % 2;
    write(new_socket, &b, sizeof(b));

    if (b == 0) {
      char r_str[1024];
      read(new_socket, r_str, 1024);
      mpz_set_str(r, r_str, 10);
      mpz_powm_ui(tmp, r, 2, user->n);
      if (mpz_cmp(tmp, x) == 0) {
        write(new_socket, "Authenticated", 14);
      } else {
        write(new_socket, "Failed", 6);
      }
    } else {
      char y_str[1024];
      read(new_socket, y_str, 1024);
      mpz_set_str(y, y_str, 10);
      mpz_powm_ui(tmp, y, 2, user->n);
      mpz_mul(tmp, tmp, user->v);
      mpz_mod(tmp, tmp, user->n);
      if (mpz_cmp(tmp, x) == 0) {
        write(new_socket, "Authenticated", 14);
      } else {
        write(new_socket, "Failed", 6);
      }
    }

    mpz_clears(x, y, r, tmp, NULL);
    close(new_socket);
  }

  return 0;
}