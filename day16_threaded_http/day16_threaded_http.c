#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

void* handle_client(void* arg)
{
    int client_fd = *(int*)arg;
    free(arg);

    char buffer[4096];

    int bytes = read(client_fd, buffer, sizeof(buffer) - 1);

    if (bytes > 0)
    {
        buffer[bytes] = '\0';

        char method[16];
        char path[256];

        sscanf(buffer, "%s %s", method, path);

        printf("[THREAD %lu] %s %s\n",
               pthread_self(),
               method,
               path);

        char response[] =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "Hello from threaded server";

        send(client_fd, response, strlen(response), 0);
    }

    close(client_fd);
    return NULL;
}

int main()
{
    int server_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 10);

    printf("Threaded HTTP server running on port 8080...\n");

    while (1)
    {
        int* client_fd = malloc(sizeof(int));

        *client_fd = accept(server_fd,
                            (struct sockaddr*)&address,
                            &addrlen);

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, client_fd);
        pthread_detach(thread);
    }

    return 0;
}
