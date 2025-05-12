#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>

int main() {
    int clientsocket, port;
    struct sockaddr_in serveraddr;
    socklen_t len;
    char from[50], to[50], mail[100], request[50], buffer[256];
   
    // Create socket
    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientsocket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    bzero((char*)&serveraddr, sizeof(serveraddr));
   
    // Get port and address
    printf("Enter the port number: ");
    scanf("%d", &port);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;
   
    // Connect to server
    if (connect(clientsocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    // Get sender and recipient information
    printf("Enter the FROM address: ");
    scanf(" %s", from); // using " %s" to avoid leftover newline issue
    printf("Enter the TO address: ");
    scanf(" %s", to);
    printf("Enter the MAIL: ");
    scanf(" %[^\n]", mail); // Use %[^\n] to read a line with spaces

    // Clear input buffer
    while (getchar() != '\n');

    printf("\nPRESS ENTER TO BEGIN THE FIRST PROCESS.\n");
    getchar(); // to consume any leftover newlines

    // HELO command
    memset(request, 0, sizeof(request));
    strcpy(request, "HELO");
    send(clientsocket, request, strlen(request), 0);
    printf("\nSending the HELO command\n");
    
    memset(buffer, 0, sizeof(buffer));
    recv(clientsocket, buffer, sizeof(buffer), 0);
    printf("The server has sent: %s\n", buffer);

    printf("\nPRESS ENTER TO SEND THE FIRST COMMAND.\n");
    getchar();

    // MAIL FROM command
    memset(request, 0, sizeof(request));
    strcpy(request, "MAIL FROM:");
    strcat(request, from);
    send(clientsocket, request, strlen(request), 0);
    printf("\nSending the MAIL FROM request\n");
    
    memset(buffer, 0, sizeof(buffer));
    recv(clientsocket, buffer, sizeof(buffer), 0);
    printf("The server has sent: %s\n", buffer);

    printf("\nPRESS ENTER TO BEGIN THE SECOND COMMAND.\n");
    getchar();

    // RCPT TO command
    memset(request, 0, sizeof(request));
    strcpy(request, "RCPT TO:");
    strcat(request, to);
    send(clientsocket, request, strlen(request), 0);
    printf("\nSending the RCPT TO request\n");
    
    memset(buffer, 0, sizeof(buffer));
    recv(clientsocket, buffer, sizeof(buffer), 0);
    printf("The server has sent: %s\n", buffer);

    printf("\nPRESS ENTER TO BEGIN THE THIRD COMMAND.\n");
    getchar();

    // DATA command
    memset(request, 0, sizeof(request));
    strcpy(request, "DATA");
    send(clientsocket, request, strlen(request), 0);
    printf("\nSending the DATA request\n");
    
    memset(buffer, 0, sizeof(buffer));
    recv(clientsocket, buffer, sizeof(buffer), 0);
    printf("The server has sent: %s\n", buffer);

    printf("\nPRESS ENTER TO BEGIN SENDING THE MAIL.\n");
    getchar();

    // Send the email body
    send(clientsocket, mail, strlen(mail), 0);
    
    memset(buffer, 0, sizeof(buffer));
    recv(clientsocket, buffer, sizeof(buffer), 0);
    printf("The server has sent: %s\n", buffer);

    // End of body, send the period "."
    memset(request, 0, sizeof(request));
    strcpy(request, ".");
    send(clientsocket, request, strlen(request), 0);
    
    memset(buffer, 0, sizeof(buffer));
    recv(clientsocket, buffer, sizeof(buffer), 0);
    printf("The server has sent: %s\n", buffer);

    printf("\nPRESS ENTER TO BEGIN THE LAST COMMAND.\n");
    getchar();

    // QUIT command
    memset(request, 0, sizeof(request));
    strcpy(request, "QUIT");
    send(clientsocket, request, strlen(request), 0);
    
    memset(buffer, 0, sizeof(buffer));
    recv(clientsocket, buffer, sizeof(buffer), 0);
    printf("The server has sent: %s\n", buffer);

    // Close connection
    close(clientsocket);
    printf("\nCLIENT CLOSING DOWN.\n");

    return 0;
}


