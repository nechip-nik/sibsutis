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

#define BUFLEN 81

int main(int argc, char* argv[]) {
    int sock;
    struct sockaddr_in servAddr, clientAddr;
    struct hostent *hp;
    char buf[BUFLEN];

    if (argc < 5) {
        printf("Usage: udpclient hostname port message repeats\n");
        exit(1);
    }

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Cannot get socket");
        exit(1);
    }

    bzero((char *) &servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp == NULL) {
        fprintf(stderr, "Unknown host: %s\n", argv[1]);
        exit(1);
    }
    bcopy(hp->h_addr, &servAddr.sin_addr, hp->h_length);
    servAddr.sin_port = htons(atoi(argv[2]));

    bzero((char *) &clientAddr, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientAddr.sin_port = htons(0); // Let the system choose a port

    if (bind(sock, (struct sockaddr *)&clientAddr, sizeof(clientAddr))) {
        perror("Client could not bind to a port");
        exit(1);
    }

    printf("CLIENT: Ready to send.\n");

    int repeats = atoi(argv[4]);

    for (int i = 0; i < repeats; ++i) {
        sleep(3);
        if (sendto(sock, argv[3], strlen(argv[3]), 0, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
            perror("Problem with sendto");
            exit(1);
        }
        socklen_t servAddrLen = sizeof(servAddr);
        if (recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *)&servAddr, &servAddrLen) < 0) {
            perror("Problem with recvfrom");
            exit(1);
        }
        printf("CLIENT: Message sent.\n");
        printf("SERVER: Сообщение: %s\n\n", buf);
    }

    close(sock);

    return 0;
}