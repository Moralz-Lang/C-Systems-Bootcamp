Day 17 was:

```text id="d17a"
Thread Pool = workers wait for jobs
```

Day 18 is:

```text id="d18a"
epoll = kernel tells you WHAT is ready
```

No polling. No wasted threads. No constant checking.

---

# 🟢 DAY 18 — epoll + Thread Pool Hybrid Server

## 🧠 Why this matters

Your current model:

### Day 17

```text id="flow17"
accept()
 ↓
queue
 ↓
worker threads
 ↓
blocking read()
```

Problem:

* threads still block on I/O
* kernel not fully leveraged
* scaling limited

---

## 🟢 Day 18 Model (REAL SERVER DESIGN)

```text id="flow18"
        epoll (kernel)
             ↓
   ready sockets only
             ↓
     worker thread pool
             ↓
     non-blocking I/O
```

---

# 🔥 Key Idea

Instead of:

```c
accept → block → read → block
```

We do:

```c
epoll_wait → ready FD → process immediately
```

---

# 📦 WHAT epoll DOES

It tracks:

* sockets
* events
* readiness state

and tells you:

```text id="ep1"
"This socket is ready to read"
"This socket has data"
```

---

# 🧠 CORE STRUCTURE

We build:

```c id="struct1"
server_fd
   ↓
epoll instance
   ↓
event loop
   ↓
worker processing
```

---

# 🟢 STEP 1 — REQUIRED HEADERS

```c id="hdr1"
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
```

---

# 🟢 STEP 2 — NON-BLOCKING SOCKETS

We MUST set:

```c id="nb1"
fcntl(fd, F_SETFL, O_NONBLOCK);
```

Why?

Because epoll expects:

```text id="nb2"
no blocking calls inside event loop
```

---

# 🟢 STEP 3 — CREATE epoll INSTANCE

```c id="ep1c"
int epoll_fd = epoll_create1(0);
```

---

# 🟢 STEP 4 — ADD SERVER SOCKET

```c id="ep2c"
struct epoll_event event;

event.events = EPOLLIN;
event.data.fd = server_fd;

epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event);
```

---

# 🟢 STEP 5 — MAIN EVENT LOOP

```c id="loop1"
struct epoll_event events[10];

while(1)
{
    int n = epoll_wait(epoll_fd,
                       events,
                       10,
                       -1);

    for(int i = 0; i < n; i++)
    {
        int fd = events[i].data.fd;

        if(fd == server_fd)
        {
            handle_accept();
        }
        else
        {
            handle_client(fd);
        }
    }
}
```

---

# 🟢 STEP 6 — ACCEPT HANDLER

```c id="acc1"
int client_fd = accept(server_fd, NULL, NULL);

fcntl(client_fd, F_SETFL, O_NONBLOCK);

struct epoll_event ev;
ev.events = EPOLLIN;
ev.data.fd = client_fd;

epoll_ctl(epoll_fd,
          EPOLL_CTL_ADD,
          client_fd,
          &ev);
```

---

# 🟢 STEP 7 — CLIENT HANDLER

```c id="cli1"
void handle_client(int fd)
{
    char buffer[4096];

    int bytes = read(fd, buffer, sizeof(buffer)-1);

    if(bytes <= 0)
    {
        close(fd);
        return;
    }

    buffer[bytes] = 0;

    printf("Client %d says: %s\n", fd, buffer);

    char response[] =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Hello from epoll server\n";

    send(fd, response, strlen(response), 0);

    close(fd);
}
```

---

# 🧠 NEW MENTAL MODEL

## BEFORE (Day 17)

```text id="m1"
Worker waits for jobs
Queue controls flow
Threads manage sockets
```

---

## NOW (Day 18)

```text id="m2"
Kernel tells you when sockets are ready
No queue needed for I/O
No blocking reads
Event-driven execution
```

---

# 🚀 FULL DAY 18 SIMPLE SERVER

## 📄 `day18_epoll_server.c`

```c id="full18"
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
```

---

# 🟢 COMPILE

```bash id="c18c"
gcc -Wall -Wextra -g day18_epoll_server.c -o day18_server
```

---

# 🟢 RUN

```bash id="r18"
./day18_server
```

---

# 🧪 TEST

```bash id="t18"
curl http://127.0.0.1:8080
```

---

# 🧠 WHAT YOU JUST LEARNED

You now understand:

### 1. epoll replaces polling

### 2. Kernel tracks readiness

### 3. No thread-per-connection needed

### 4. Event-driven architecture

### 5. Non-blocking sockets
