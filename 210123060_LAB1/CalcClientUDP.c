#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include<unistd.h>
#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Server address configuration
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        exit(1);
    }

    while (1) {
        double num1, num2;
        char operator, buffer[BUFFER_SIZE];

        printf("Enter an arithmetic operation (e.g., 5 + 3): ");
        scanf("%lf %c %lf", &num1, &operator, &num2);

        // Send calculation request to server
        snprintf(buffer, BUFFER_SIZE, "%.2lf %c %.2lf", num1, operator, num2);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        // Receive and display the result from the server
        ssize_t recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (recv_len == -1) {
            perror("Error in receiving data");
            continue;
        }

        buffer[recv_len] = '\0';
        printf("Result: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
