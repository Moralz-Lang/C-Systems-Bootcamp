#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

int main()
{
    int server_fd, client_fd, max_fd;

    struct sockaddr_in address;

    socklen_t addrlen = sizeof(address);

    char buffer[1024];

    fd_set master_set, read_set;

    // CREATE SERVER SOCKET
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // CONFIGURE ADDRESS
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // BIND SOCKET
    bind(server_fd,
         (struct sockaddr*)&address,
         sizeof(address));

    // LISTEN FOR CLIENTS
    listen(server_fd, 5);

    FD_ZERO(&master_set);

    FD_SET(server_fd, &master_set);

    max_fd = server_fd;

    printf("Select server running on port 8080...\n");

    while(1)
    {
        // COPY FD SET
        read_set = master_set;

        // WAIT FOR EVENTS
        select(max_fd + 1,
               &read_set,
               NULL,
               NULL,
               NULL);

        for(int fd = 0; fd <= max_fd; fd++)
        {
            if(FD_ISSET(fd, &read_set))
            {
                // NEW CLIENT
                if(fd == server_fd)
                {
                    client_fd = accept(server_fd,
                                       (struct sockaddr*)&address,
                                       &addrlen);

                    FD_SET(client_fd, &master_set);

                    if(client_fd > max_fd)
                    {
                        max_fd = client_fd;
                    }

                    printf("New client connected: %d\n",
                           client_fd);
                }
                else
                {
                    int bytes = read(fd,
                                     buffer,
                                     sizeof(buffer));

                    if(bytes <= 0)
                    {
                        close(fd);

                        FD_CLR(fd, &master_set);

                        printf("Client disconnected: %d\n",
                               fd);
                    }
                    else
                    {
                        buffer[bytes] = '\0';

                        printf("Client %d: %s\n",
                               fd,
                               buffer);

                        send(fd,
                             buffer,
                             bytes,
                             0);
                    }
                }
            }
        }
    }

    return 0;
}
