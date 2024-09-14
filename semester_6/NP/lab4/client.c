#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUF_SIZE = 64;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port> <i>\n", argv[0]);
        return 1;
    }
    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    int i = atoi(argv[3]);

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Failed to create socket");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return 1;
    }
    memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Failed to connect");
        close(sockfd);
        return 1;
    }

    // Send data to server
    char buf[BUF_SIZE];
    snprintf(buf, BUF_SIZE, "%d", i);
    while (1) {
        if (send(sockfd, buf, strlen(buf), 0) == -1) {
            perror("Failed to send message");
            close(sockfd);
            return 1;
        }
        printf("Message %d sent to server\n", i);
        sleep(i);
    }

    close(sockfd);
    return 0;
}