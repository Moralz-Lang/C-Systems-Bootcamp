#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <pthread.h>

#include <sys/epoll.h>
#include <fcntl.h>

#define PORT 8080
#define THREAD_COUNT 4
#define QUEUE_SIZE 100
#define MAX_EVENTS 10
#define MAX_REQUEST 4096

/* ---------------- QUEUE ---------------- */

int queue[QUEUE_SIZE];
int head = 0;
int tail = 0;

pthread_mutex_t queue_lock;
pthread_cond_t queue_cond;

/* ---------------- HTTP PARSER ---------------- */

void parse_request(char *buffer, char *method, char *path)
{
    sscanf(buffer, "%15s %255s", method, path);
}

/* ---------------- ROUTER ---------------- */

void route_request(int fd, const char *path)
{
    if(strcmp(path, "/") == 0)
    {
        send(fd,
            "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
            "<h1>Home Page</h1>",
            strlen("<h1>Home Page</h1>") + 58,
            0);
    }
    else if(strcmp(path, "/about") == 0)
    {
        send(fd,
            "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
            "<h1>About Page</h1>",
            strlen("<h1>About Page</h1>") + 58,
            0);
    }
    else if(strcmp(path, "/api") == 0)
    {
        send(fd,
            "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n"
            "{\"status\":\"ok\"}",
            strlen("{\"status\":\"ok\"}") + 60,
            0);
    }
    else
    {
        send(fd,
            "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nNot Found",
            strlen("Not Found") + 62,
            0);
    }
}

/* ---------------- WORKER THREADS ---------------- */

void* worker(void* arg)
{
    (void)arg;

    while(1)
    {
        pthread_mutex_lock(&queue_lock);

        while(head == tail)
        {
            pthread_cond_wait(&queue_cond, &queue_lock);
        }

        int fd = queue[head];
        head = (head + 1) % QUEUE_SIZE;

        pthread_mutex_unlock(&queue_lock);

        char buffer[MAX_REQUEST];

        int bytes = read(fd, buffer, sizeof(buffer) - 1);

        if(bytes <= 0)
        {
            close(fd);
            continue;
        }

        buffer[bytes] = 0;

        char method[16];
        char path[256];

        parse_request(buffer, method, path);

        printf("[THREAD %lu] %s %s\n",
               pthread_self(),
               method,
               path);

        route_request(fd, path);

        close(fd);
    }

    return NULL;
}

/* ---------------- MAIN ---------------- */

int main()
{
    int server_fd, epoll_fd;
    struct sockaddr_in addr;

    pthread_t threads[THREAD_COUNT];

    pthread_mutex_init(&queue_lock, NULL);
    pthread_cond_init(&queue_cond, NULL);

    /* CREATE THREAD POOL */
    for(int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_create(&threads[i], NULL, worker, NULL);
    }

    /* SOCKET SETUP */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 10);

    fcntl(server_fd, F_SETFL, O_NONBLOCK);

    /* EPOLL */
    epoll_fd = epoll_create1(0);

    struct epoll_event ev, events[MAX_EVENTS];

    ev.events = EPOLLIN;
    ev.data.fd = server_fd;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);

    printf("Day 20 Server running on port %d\n", PORT);

    /* EVENT LOOP */
    while(1)
    {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for(int i = 0; i < n; i++)
        {
            int fd = events[i].data.fd;

            if(fd == server_fd)
            {
                int client_fd = accept(server_fd, NULL, NULL);

                if(client_fd < 0)
                    continue;

                fcntl(client_fd, F_SETFL, O_NONBLOCK);

                pthread_mutex_lock(&queue_lock);

                queue[tail] = client_fd;
                tail = (tail + 1) % QUEUE_SIZE;

                pthread_cond_signal(&queue_cond);

                pthread_mutex_unlock(&queue_lock);
            }
        }
    }

    return 0;
}
