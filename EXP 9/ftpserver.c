#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, client_fd, port;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char filename[100], buffer[1024];
    FILE *fp;

    printf("Enter port: ");
    scanf("%d", &port);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }
    printf("Socket created successfully\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }
    printf("Bind successful\n");

    listen(server_fd, 5);
    printf("Waiting for client...\n");

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("Accept failed");
        return 1;
    }
    printf("Client connected\n");

    recv(client_fd, filename, sizeof(filename), 0);
    printf("Requested file: %s\n", filename);

    fp = fopen(filename, "r");
    if (fp == NULL) {
        send(client_fd, "error", 5, 0);
    } else {
        while (fgets(buffer, sizeof(buffer), fp)) {
            send(client_fd, buffer, strlen(buffer), 0);
            usleep(500000);
        }
        send(client_fd, "completed", 9, 0);
        fclose(fp);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

