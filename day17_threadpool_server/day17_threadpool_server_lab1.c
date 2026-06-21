#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <pthread.h>

#define PORT 8080

#define THREAD_COUNT 4

#define QUEUE_SIZE 100

#define MAX_REQUEST 4096

int queue[QUEUE_SIZE];

int head = 0;
int tail = 0;

pthread_mutex_t queue_lock;

pthread_cond_t queue_cond;

void handle_request(int client_fd)
{
    char buffer[MAX_REQUEST];

    int bytes =
        read(client_fd,
             buffer,
             sizeof(buffer)-1);

    if(bytes <= 0)
    {
        close(client_fd);
        return;
    }

    buffer[bytes] = '\0';

    char method[16];
    char path[256];

    sscanf(buffer,
           "%15s %255s",
           method,
           path);

    printf("[THREAD %lu] %s %s\n",
           pthread_self(),
           method,
           path);

    /*
       SECURITY CHECK #1
       ONLY ALLOW GET
    */

    if(strcmp(method, "GET") != 0)
    {
        close(client_fd);
        return;
    }

    /*
       SECURITY CHECK #2
       BLOCK PATH TRAVERSAL
    */

    if(strstr(path, ".."))
    {
        close(client_fd);
        return;
    }

    char response[] =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Hello from Day 17 Thread Pool Server\n";

    send(client_fd,
         response,
         strlen(response),
         0);

    close(client_fd);
}

void* worker(void* arg)
{
    (void)arg;

    while(1)
    {
        pthread_mutex_lock(&queue_lock);

        while(head == tail)
        {
            pthread_cond_wait(
                &queue_cond,
                &queue_lock);
        }

        int client_fd = queue[head];

        head =
            (head + 1) % QUEUE_SIZE;

        pthread_mutex_unlock(
            &queue_lock);

        handle_request(client_fd);
    }

    return NULL;
}

int main()
{
    int server_fd;

    struct sockaddr_in address;

    socklen_t addrlen =
        sizeof(address);

    pthread_t workers[THREAD_COUNT];

    pthread_mutex_init(
        &queue_lock,
        NULL);

    pthread_cond_init(
        &queue_cond,
        NULL);

    /*
       CREATE WORKERS
    */

    for(int i = 0;
        i < THREAD_COUNT;
        i++)
    {
        pthread_create(
            &workers[i],
            NULL,
            worker,
            NULL);
    }

    server_fd =
        socket(AF_INET,
               SOCK_STREAM,
               0);

    address.sin_family =
        AF_INET;

    address.sin_addr.s_addr =
        INADDR_ANY;

    address.sin_port =
        htons(PORT);

    bind(server_fd,
         (struct sockaddr*)&address,
         sizeof(address));

    listen(server_fd, 10);

    printf(
      "Day 17 Thread Pool Server Running\n");

    while(1)
    {
        int client_fd =
            accept(server_fd,
                   (struct sockaddr*)&address,
                   &addrlen);

        pthread_mutex_lock(
            &queue_lock);

        queue[tail] = client_fd;

        tail =
            (tail + 1)
            % QUEUE_SIZE;

        pthread_cond_signal(
            &queue_cond);

        pthread_mutex_unlock(
            &queue_lock);
    }

    return 0;
}
