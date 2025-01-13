#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define MSG_LEN 1024

void* handle_client(void* arg) {
    int client_socket = *((int*)arg);
    char buffer[MSG_LEN];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        printf("Received encoded message: %s\n", buffer);

        // Send acknowledgment
        char ack_msg[] = "2 Acknowledgment";
        send(client_socket, ack_msg, sizeof(ack_msg), 0);
    }

    close(client_socket);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Server_Port_Number>\n", argv[0]);
        return 1;
    }

    int server_port = atoi(argv[1]);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        return 1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Error binding to address");
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, 5) == -1) {
        perror("Error listening on socket");
        close(server_socket);
        return 1;
    }

    printf("Server listening on port %d\n", server_port);

    while (1) {
        struct sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_size);
        if (client_socket == -1) {
            perror("Error accepting connection");
            continue;
        }

        printf("Client connected.\n");
        int* client_socket_ptr = (int* )malloc(sizeof(int));
        *client_socket_ptr=client_socket;
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, client_socket_ptr);
        pthread_detach(thread_id);
    }

    close(server_socket);
    return 0;
}
