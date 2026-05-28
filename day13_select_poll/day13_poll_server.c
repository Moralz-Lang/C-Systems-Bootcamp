#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <string.h>

int main()
{
    int server_fd, client_fd;

    struct sockaddr_in address;

    socklen_t addrlen = sizeof(address);

    char buffer[1024];

    struct pollfd fds[1024];

    // CREATE SOCKET
    server_fd = socket(AF_INET,
                       SOCK_STREAM,
                       0);

    // CONFIGURE ADDRESS
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // BIND SOCKET
    bind(server_fd,
         (struct sockaddr*)&address,
         sizeof(address));

    // LISTEN
    listen(server_fd, 5);

    // REGISTER SERVER FD
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    int nfds = 1;

    printf("Poll server running on port 8080...\n");

    while(1)
    {
        poll(fds,
             nfds,
             -1);

        for(int i = 0; i < nfds; i++)
        {
            if(fds[i].revents & POLLIN)
            {
                // NEW CLIENT
                if(fds[i].fd == server_fd)
                {
                    client_fd = accept(server_fd,
                                       (struct sockaddr*)&address,
                                       &addrlen);

                    fds[nfds].fd = client_fd;
                    fds[nfds].events = POLLIN;

                    nfds++;

                    printf("New client connected: %d\n",
                           client_fd);
                }
                else
                {
                    int bytes = read(fds[i].fd,
                                     buffer,
                                     sizeof(buffer));

                    if(bytes <= 0)
                    {
                        close(fds[i].fd);

                        fds[i] = fds[nfds - 1];

                        nfds--;

                        printf("Client disconnected\n");
                    }
                    else
                    {
                        buffer[bytes] = '\0';

                        printf("Client says: %s\n",
                               buffer);

                        send(fds[i].fd,
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
