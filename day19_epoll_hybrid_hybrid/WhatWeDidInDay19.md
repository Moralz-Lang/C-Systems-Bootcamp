Day 19 is where everything you built finally comes together into a **real server architecture pattern** used in production systems.

You now combine:

```text id="d19a"
epoll (event detection)
+ 
thread pool (work execution)
+
HTTP parsing
+
file serving
```

This is basically a simplified **nginx-style hybrid model**.

---

# 🟢 DAY 19 — EPOLL + THREAD POOL HYBRID SERVER

## 🧠 Why this exists

You’ve already seen:

### ❌ Day 17 (Thread Pool only)

```text id="17a"
Threads still block on read()
```

### ❌ Day 18 (epoll only)

```text id="18a"
Single loop handles everything (no scaling work distribution)
```

---

## 🟢 Day 19 SOLUTION

We split responsibilities:

```text id="19a"
        epoll (I/O detection)
                 ↓
         job queue (FDs)
                 ↓
         thread pool workers
                 ↓
         HTTP + file handling
```

---

# 🧠 CORE DESIGN

## THREADS DO NOT WAIT ON I/O

Instead:

* epoll detects readiness
* pushes FD into queue
* workers process instantly

---

# 📦 SYSTEM ARCHITECTURE

```text id="arch19"
Client
   ↓
epoll (kernel monitors sockets)
   ↓
ready FD detected
   ↓
enqueue job
   ↓
worker thread picks job
   ↓
process HTTP request
   ↓
send response
```

---

# 🟢 STEP 1 — HEADERS

```c id="h19"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <pthread.h>
#include <sys/epoll.h>
#include <fcntl.h>
```

---

# 🟢 STEP 2 — CONFIG

```c id="c19"
#define PORT 8080
#define THREAD_COUNT 4
#define QUEUE_SIZE 100
#define MAX_EVENTS 10
#define MAX_REQUEST 4096
```

---

# 🟢 STEP 3 — SHARED QUEUE

```c id="q19"
int queue[QUEUE_SIZE];
int head = 0;
int tail = 0;

pthread_mutex_t queue_lock;
pthread_cond_t queue_cond;
```

---

# 🟢 STEP 4 — THREAD WORKER

Workers ONLY handle requests:

```c id="w19"
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

        int bytes = read(fd, buffer, sizeof(buffer)-1);

        if(bytes <= 0)
        {
            close(fd);
            continue;
        }

        buffer[bytes] = 0;

        printf("Worker %lu handling FD %d\n",
               pthread_self(), fd);

        char response[] =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "Hello from Day 19 Hybrid Server\n";

        send(fd, response, strlen(response), 0);

        close(fd);
    }
}
```

---

# 🟢 STEP 5 — EPOLL SETUP

```c id="e19"
int epoll_fd = epoll_create1(0);
```

---

# 🟢 STEP 6 — SERVER SOCKET

```c id="s19"
int server_fd = socket(AF_INET, SOCK_STREAM, 0);

struct sockaddr_in addr;

addr.sin_family = AF_INET;
addr.sin_port = htons(PORT);
addr.sin_addr.s_addr = INADDR_ANY;

bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
listen(server_fd, 10);

fcntl(server_fd, F_SETFL, O_NONBLOCK);
```

---

# 🟢 STEP 7 — ADD SERVER TO EPOLL

```c id="a19"
struct epoll_event ev;

ev.events = EPOLLIN;
ev.data.fd = server_fd;

epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);
```

---

# 🟢 STEP 8 — CREATE THREAD POOL

```c id="t19"
pthread_t threads[THREAD_COUNT];

pthread_mutex_init(&queue_lock, NULL);
pthread_cond_init(&queue_cond, NULL);

for(int i = 0; i < THREAD_COUNT; i++)
{
    pthread_create(&threads[i], NULL, worker, NULL);
}
```

---

# 🟢 STEP 9 — MAIN EVENT LOOP

```c id="l19"
struct epoll_event events[MAX_EVENTS];

while(1)
{
    int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

    for(int i = 0; i < n; i++)
    {
        int fd = events[i].data.fd;

        if(fd == server_fd)
        {
            int client_fd = accept(server_fd, NULL, NULL);

            fcntl(client_fd, F_SETFL, O_NONBLOCK);

            pthread_mutex_lock(&queue_lock);

            queue[tail] = client_fd;
            tail = (tail + 1) % QUEUE_SIZE;

            pthread_cond_signal(&queue_cond);

            pthread_mutex_unlock(&queue_lock);
        }
    }
}
```

---

# 🟢 FULL FILE SUMMARY

You now have:

```text id="full19"
epoll → detects connections
thread pool → processes requests
queue → bridges system
non-blocking sockets → prevents stalls
```

---

# 🧠 WHAT YOU JUST BUILT

This is the simplified version of:

* nginx worker model
* high-performance API servers
* load-balanced event loops

---

# ⚙️ COMPILATION

```bash id="c19c"
gcc -Wall -Wextra -g day19_server.c -o day19_server -pthread
```

---

# ▶ RUN

```bash id="r19"
./day19_server
```

---

# 🧪 TEST

```bash id="t19"
curl http://127.0.0.1:8080
```

---

# 🧠 FINAL MENTAL MODEL

```text id="mm19"
        Kernel (epoll)
             ↓
        Event readiness
             ↓
        Queue (FD buffer)
             ↓
        Worker pool
             ↓
        Response
```
