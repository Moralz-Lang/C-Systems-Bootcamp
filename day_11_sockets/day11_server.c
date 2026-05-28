#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{
    int server_fd, client_fd;

    struct sockaddr_in address;

    socklen_t addrlen = sizeof(address);

    char buffer[1024] = {0};

    char *message = "Hello from server";

    // CREATE SOCKET
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // CONFIGURE ADDRESS
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // BIND SOCKET TO PORT
    bind(server_fd,
         (struct sockaddr*)&address,
         sizeof(address));

    // LISTEN FOR CONNECTIONS
    listen(server_fd, 3);

    printf("Server listening on port 8080...\n");

    // ACCEPT CLIENT
    client_fd = accept(server_fd,
                       (struct sockaddr*)&address,
                       &addrlen);

    // READ CLIENT DATA
    read(client_fd, buffer, sizeof(buffer));

    printf("Client says: %s\n", buffer);

    // SEND RESPONSE
    send(client_fd, message, strlen(message), 0);

    // CLEANUP
    close(client_fd);
    close(server_fd);

    return 0;
}
