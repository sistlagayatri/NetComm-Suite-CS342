#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void performCalculation(int newsockfd) {
    int num1, num2, ans, choice;

    while (1) {
        write(newsockfd, "Enter Number 1: ", strlen("Enter Number 1: "));
        read(newsockfd, &num1, sizeof(int));
        printf("Client Number 1 is: %d\n", num1);

        write(newsockfd, "Enter Number 2: ", strlen("Enter Number 2: "));
        read(newsockfd, &num2, sizeof(int));
        printf("Client Number 2 is: %d\n", num2);

        write(newsockfd, "Enter Your Choice\n1.Add\n2.Subtract\n3.Division\n4.Multiplication\n5.Exit", strlen("Enter Your Choice"));
        read(newsockfd, &choice, sizeof(int));
        printf("Client - Choice is: %d\n", choice);

        switch (choice) {
            case 1:
                ans = num1 + num2;
                break;
            case 2:
                ans = num1 - num2;
                break;
            case 3:
                if (num2 != 0) {
                    ans = num1 / num2;
                } else {
                    ans = 0; // Handle division by zero
                }
                break;
            case 4:
                ans = num1 * num2;
                break;
            case 5:
                close(newsockfd);
                return;
            default:
                ans = 0; // Invalid choice
                break;
        }

        write(newsockfd, &ans, sizeof(int));
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Port not provided. Program Terminated\n");
        exit(1);
    }

    int sockfd, newsockfd, portno;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening socket");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));

    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("Binding Failed");
    }

    listen(sockfd, SOMAXCONN);

    while (1) {
        struct sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("Error on Accept");
            continue; // Continue to the next iteration of the loop
        }

        performCalculation(newsockfd);

        close(newsockfd);
    }

    close(sockfd);
    return 0;
}
