#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void communicateWithServer(int sockfd) {
    int num1, num2, choice, ans;
    char buffer[256];

    while (1) {
        bzero(buffer, sizeof(buffer));
        read(sockfd, buffer, sizeof(buffer));
        printf("Server - %s\n", buffer);

        scanf("%d", &num1);
        write(sockfd, &num1, sizeof(int));

        bzero(buffer, sizeof(buffer));
        read(sockfd, buffer, sizeof(buffer));
        printf("Server - %s\n", buffer);

        scanf("%d", &num2);
        write(sockfd, &num2, sizeof(int));

        bzero(buffer, sizeof(buffer));
        read(sockfd, buffer, sizeof(buffer));
        printf("Server - %s\n", buffer);

        scanf("%d", &choice);
        write(sockfd, &choice, sizeof(int));

        if (choice == 5) {
            printf("You have selected to exit. Exiting...\n");
            break;
        }

        read(sockfd, &ans, sizeof(int));
        printf("Server: The answer is: %d\n", ans);

        if (choice != 5) {
            continue;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        exit(1);
    }

    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        error("Error opening socket");
    }

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "No such host\n");
        exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("Connection Failed");
    }

    communicateWithServer(sockfd);

    close(sockfd);
    return 0;
}
