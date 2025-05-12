#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_fd, port;
    struct sockaddr_in server_addr;
    char filename[100], new_filename[100], buffer[1024];
    FILE *fp;

    printf("Enter port: ");
    scanf("%d", &port);

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }
    printf("Socket created successfully\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }
    printf("Connected to server\n");

    printf("Enter existing file name: ");
    scanf("%s", filename);
    printf("Enter new file name: ");
    scanf("%s", new_filename);

    send(client_fd, filename, sizeof(filename), 0);
    fp = fopen(new_filename, "w");
    if (!fp) {
        perror("File creation failed");
        return 1;
    }

    while (1) {
        int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) break;

        buffer[bytes_received] = '\0';
        if (strcmp(buffer, "error") == 0) {
            printf("File not available\n");
            break;
        }
        if (strcmp(buffer, "completed") == 0) {
            printf("File transfer completed\n");
            break;
        }
        fputs(buffer, fp);
        printf("%s", buffer);
    }

    fclose(fp);
    close(client_fd);
    return 0;
}

