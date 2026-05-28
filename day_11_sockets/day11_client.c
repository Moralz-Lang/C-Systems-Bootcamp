#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{
    int sock;

    struct sockaddr_in serv_addr;

    char *message = "Hello from client";

    char buffer[1024] = {0};

    // CREATE SOCKET
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // CONVERT IP STRING TO BINARY
    inet_pton(AF_INET,
              "127.0.0.1",
              &serv_addr.sin_addr);

    // CONNECT TO SERVER
    connect(sock,
            (struct sockaddr*)&serv_addr,
            sizeof(serv_addr));

    // SEND DATA
    send(sock, message, strlen(message), 0);

    // READ RESPONSE
    read(sock, buffer, sizeof(buffer));

    printf("Server says: %s\n", buffer);

    close(sock);

    return 0;
}
