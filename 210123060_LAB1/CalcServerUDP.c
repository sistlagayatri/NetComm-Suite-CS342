#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include<unistd.h>
#define PORT 12345
#define BUFFER_SIZE 1024

double performCalculation(double a, double b, char operator) {
    switch (operator) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            if (b != 0)
                return a / b;
            else
                return -1;  // Indicate division by zero error
        default:
            return -2; // Indicate unsupported operator error
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Server address configuration
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to the address and port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(1);
    }

    printf("UDP Calculator Server is listening on port %d...\n", PORT);

    while (1) {
        double num1, num2, result;
        char operator;

        // Receive calculation request from client
        ssize_t recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        if (recv_len == -1) {
            perror("Error in receiving data");
            continue;
        }

        sscanf(buffer, "%lf %c %lf", &num1, &operator, &num2);

        // Perform calculation
        result = performCalculation(num1, num2, operator);

        // Send the result back to the client
        snprintf(buffer, BUFFER_SIZE, "%.2lf", result);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, client_len);
    }

    close(sockfd);
    return 0;
}
