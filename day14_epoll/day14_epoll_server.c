#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define MAX_EVENTS 64

int main()
{
    int server_fd, client_fd;

    struct sockaddr_in address;

    socklen_t addrlen = sizeof(address);

    char buffer[1024];

    // CREATE SERVER SOCKET
    server_fd = socket(AF_INET,
                       SOCK_STREAM,
                       0);

    // CONFIGURE SERVER ADDRESS
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // BIND SOCKET
    bind(server_fd,
         (struct sockaddr*)&address,
         sizeof(address));

    // LISTEN FOR CONNECTIONS
    listen(server_fd, 5);

    // CREATE EPOLL INSTANCE
    int epoll_fd = epoll_create1(0);

    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];

    // REGISTER SERVER SOCKET
    event.events = EPOLLIN;
    event.data.fd = server_fd;

    epoll_ctl(epoll_fd,
              EPOLL_CTL_ADD,
              server_fd,
              &event);

    printf("epoll server running on port 8080...\n");

    while(1)
    {
        // WAIT FOR EVENTS
        int nfds = epoll_wait(epoll_fd,
                              events,
                              MAX_EVENTS,
                              -1);

        for(int i = 0; i < nfds; i++)
        {
            // NEW CLIENT CONNECTION
            if(events[i].data.fd == server_fd)
            {
                client_fd = accept(server_fd,
                                   (struct sockaddr*)&address,
                                   &addrlen);

                event.events = EPOLLIN;
                event.data.fd = client_fd;

                epoll_ctl(epoll_fd,
                          EPOLL_CTL_ADD,
                          client_fd,
                          &event);

                printf("New client connected: %d\n",
                       client_fd);
            }
            else
            {
                int fd = events[i].data.fd;

                int bytes = read(fd,
                                 buffer,
                                 sizeof(buffer));

                if(bytes <= 0)
                {
                    close(fd);

                    epoll_ctl(epoll_fd,
                              EPOLL_CTL_DEL,
                              fd,
                              NULL);

                    printf("Client disconnected: %d\n",
                           fd);
                }
                else
                {
                    buffer[bytes] = '\0';

                    printf("Client %d says: %s\n",
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

    close(server_fd);

    close(epoll_fd);

    return 0;
}
