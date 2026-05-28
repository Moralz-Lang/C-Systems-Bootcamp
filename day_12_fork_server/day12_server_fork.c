#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>

int main()
{
    int server_fd, client_fd;

    struct sockaddr_in address;

    int addrlen = sizeof(address);

    char buffer[1024] = {0};

    char *msg = "Hello from forked server";

    // CREATE SOCKET
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // CONFIGURE SERVER ADDRESS
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // BIND SOCKET
    bind(server_fd,
         (struct sockaddr*)&address,
         sizeof(address));

    // LISTEN FOR CLIENTS
    listen(server_fd, 5);

    printf("Fork server running on port 8080...\n");

    while(1)
    {
        // ACCEPT CLIENT
        client_fd = accept(server_fd,
                           (struct sockaddr*)&address,
                           (socklen_t*)&addrlen);

        if(client_fd < 0)
        {
            continue;
        }

        // CREATE CHILD PROCESS
        pid_t pid = fork();

        // CHILD PROCESS
        if(pid == 0)
        {
            close(server_fd);

            read(client_fd,
                 buffer,
                 sizeof(buffer));

            printf("[Child %d] Client says: %s\n",
                   getpid(),
                   buffer);

            send(client_fd,
                 msg,
                 strlen(msg),
                 0);

            close(client_fd);

            return 0;
        }
        else
        {
            // PARENT PROCESS
            close(client_fd);

            // CLEANUP ZOMBIES
            waitpid(-1,
                    NULL,
                    WNOHANG);
        }
    }

    return 0;
}
