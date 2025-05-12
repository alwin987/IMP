#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>

int main() {
    int serversocket, port;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len;
    char request[50], from[50], to[50], mail[100];

    // Create socket
    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    bzero((char*)&serveraddr, sizeof(serveraddr));

    // Get port number and bind
    printf("Enter the port number: ");
    scanf("%d", &port);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;
   
    // Set socket option to reuse address
    int opt = 1;
    if (setsockopt(serversocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Setsockopt failed");
        return 1;
    }

    if (bind(serversocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("Binding failed");
        return 1;
    }

    listen(serversocket, 5);
    len = sizeof(clientaddr);
   
    printf("Waiting for the request from the client.\n");

    int newsocket = accept(serversocket, (struct sockaddr*)&clientaddr, &len);
    if (newsocket < 0) {
        perror("Connection failed");
        return 1;
    }

    // Server waits for HELO command
    memset(request, 0, sizeof(request));
    recv(newsocket, request, sizeof(request), 0);
    printf("The client has sent: %s\n", request);
    strcpy(request, "250 REQUEST COMMAND COMPLETED.");
    send(newsocket, request, strlen(request), 0);

    // Server waits for MAIL FROM command
    memset(request, 0, sizeof(request));
    recv(newsocket, request, sizeof(request), 0);
    printf("The client has sent: %s\n", request);
    strcpy(request, "250 REQUEST COMMAND COMPLETED.");
    send(newsocket, request, strlen(request), 0);

    // Server waits for RCPT TO command
    memset(request, 0, sizeof(request));
    recv(newsocket, request, sizeof(request), 0);
    printf("The client has sent: %s\n", request);
    strcpy(request, "250 REQUEST COMMAND COMPLETED.");
    send(newsocket, request, strlen(request), 0);

    // Server waits for DATA command
    memset(request, 0, sizeof(request));
    recv(newsocket, request, sizeof(request), 0);
    printf("The client has sent: %s\n", request);
    strcpy(request, "354 START MAIL INPUT.");
    send(newsocket, request, strlen(request), 0);

    // Server waits for email body
    memset(mail, 0, sizeof(mail));
    recv(newsocket, mail, sizeof(mail), 0);
    printf("The client has sent the mail: %s\n", mail);
    strcpy(request, "250 REQUEST COMMAND COMPLETED.");
    send(newsocket, request, strlen(request), 0);

    // End of message body
    memset(request, 0, sizeof(request));
    recv(newsocket, request, sizeof(request), 0);
    if (strcmp(request, ".") == 0) {
        strcpy(request, "250 REQUEST COMMAND COMPLETED.");
        send(newsocket, request, strlen(request), 0);
    }

    // Server waits for QUIT command
    memset(request, 0, sizeof(request));
    recv(newsocket, request, sizeof(request), 0);
    printf("The client has sent: %s\n", request);
    strcpy(request, "221 SERVICE CLOSING.");
    send(newsocket, request, strlen(request), 0);

    // Close connection
    close(newsocket);
    close(serversocket);
    printf("SERVER SHUTTING DOWN.\n");

    return 0;
}
