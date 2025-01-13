#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>

#define MSG_LEN 1024

void encodeBase64(const char* input, char* output) {
    static const char encoding_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int input_length = strlen(input);
    int output_length = 4 * ((input_length + 2) / 3);

    for (int i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? (unsigned char)input[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)input[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)input[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        output[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        output[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        output[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        output[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < (3 - (input_length % 3)) % 3; i++) {
        output[output_length - 1 - i] = '=';
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Server_IP_Address> <Server_Port_Number>\n", argv[0]);
        return 1;
    }

    const char* server_ip = argv[1];
    int server_port = atoi(argv[2]);

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        return 1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &(server_address.sin_addr));

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Error connecting to server");
        close(client_socket);
        return 1;
    }

    char input_msg[MSG_LEN];
    while (1) {
        printf("Enter a message (or 'exit' to close): ");
        fgets(input_msg, sizeof(input_msg), stdin);
        input_msg[strlen(input_msg) - 1] = '\0'; // Remove newline

        if (strcmp(input_msg, "exit") == 0) {
            break;
        }

        char encoded_msg[MSG_LEN * 4 / 3 + 4];
        encodeBase64(input_msg, encoded_msg);

        send(client_socket, encoded_msg, strlen(encoded_msg), 0);

        char ack_buffer[MSG_LEN];
        int bytes_received = recv(client_socket, ack_buffer, sizeof(ack_buffer), 0);
        if (bytes_received > 0) {
            ack_buffer[bytes_received] = '\0';
            printf("Received acknowledgment: %s\n", ack_buffer);
        } else {
            perror("Error receiving acknowledgment");
            break;
        }
    }

    close(client_socket);
    return 0;
}
