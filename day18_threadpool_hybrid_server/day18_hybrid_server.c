#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <sys/epoll.h>
#include <fcntl.h>

#define PORT 8080
#define MAX_EVENTS 10

int main()
{
    int server_fd;
    struct sockaddr_in addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd,
         (struct sockaddr*)&addr,
         sizeof(addr));

    listen(server_fd, 10);

    fcntl(server_fd, F_SETFL, O_NONBLOCK);

    int epoll_fd = epoll_create1(0);

    struct epoll_event ev, events[MAX_EVENTS];

    ev.events = EPOLLIN;
    ev.data.fd = server_fd;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);

    printf("Day 18 epoll server running...\n");

    while(1)
    {
        int n = epoll_wait(epoll_fd,
                           events,
                           MAX_EVENTS,
                           -1);

        for(int i = 0; i < n; i++)
        {
            int fd = events[i].data.fd;

            if(fd == server_fd)
            {
                int client_fd =
                    accept(server_fd, NULL, NULL);

                fcntl(client_fd, F_SETFL, O_NONBLOCK);

                struct epoll_event client_ev;

                client_ev.events = EPOLLIN;
                client_ev.data.fd = client_fd;

                epoll_ctl(epoll_fd,
                          EPOLL_CTL_ADD,
                          client_fd,
                          &client_ev);
            }
            else
            {
                char buffer[4096];

                int bytes =
                    read(fd, buffer, sizeof(buffer)-1);

                if(bytes <= 0)
                {
                    close(fd);
                    continue;
                }

                buffer[bytes] = 0;

                printf("Client %d: %s\n", fd, buffer);

                char response[] =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/plain\r\n"
                    "\r\n"
                    "Hello from epoll server\n";

                send(fd, response, strlen(response), 0);

                close(fd);
            }
        }
    }

    return 0;
}
