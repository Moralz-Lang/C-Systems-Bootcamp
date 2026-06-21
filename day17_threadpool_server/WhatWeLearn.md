# 🟢 DAY 17 — THREAD POOLS + SECURE REQUEST HANDLING

---

# Why Day 16 Doesn't Scale

Day 16 creates:

```c
pthread_create()
```

for every client.

Problems:

### Thread Explosion

100 clients

↓

100 threads

1000 clients

↓

1000 threads

5000 clients

↓

server collapse

---

### Memory Consumption

Each thread has:

```text
Stack
Registers
Scheduler state
Thread metadata
```

often multiple megabytes.

---

### Context Switching

CPU constantly switches:

```text
Thread A
↓
Thread B
↓
Thread C
↓
Thread D
```

instead of doing useful work.

---

# Production Solution

Create workers ONCE.

Reuse forever.

---

# Architecture

```text
                 +----------------+
Client --------> | Accept Thread  |
                 +----------------+
                         |
                         v
                 +----------------+
                 | Job Queue      |
                 +----------------+
                    |    |    |
                    v    v    v

                Worker Worker Worker
                 #1     #2     #3

```

---

# Thread Pool Concept

Create:

```c
#define THREAD_COUNT 4
```

Server starts:

```text
Worker 1
Worker 2
Worker 3
Worker 4
```

and they wait.

No new threads are created.

Ever.

---

# Job Queue

Store client sockets:

```c
int queue[100];
```

When client arrives:

```c
queue[tail] = client_fd;
```

Worker pulls:

```c
client_fd = queue[head];
```

Processes request.

Returns to waiting state.

---

# Real World Usage

This model exists in:

* Apache worker MPM
* Java application servers
* nginx thread pools
* enterprise backend services

---

# Security Hardening Begins

Day 15 HTTP server had issues.

Example:

```http
GET /../../etc/passwd
```

Your server would attempt:

```text
www/../../etc/passwd
```

Very bad.

---

# Attack #1 — Path Traversal

Attacker:

```http
GET /../../../etc/shadow
```

Goal:

```text
escape web root
```

Defense:

Reject:

```text
..
```

---

## Validation Example

```c
if (strstr(path, ".."))
{
    close(client_fd);
    return;
}
```

---

# Attack #2 — Oversized Requests

Bad request:

```text
50 MB header
```

Could crash server.

Limit request size.

---

Example:

```c
#define MAX_REQUEST 4096
```

Only read:

```c
read(fd, buffer, MAX_REQUEST);
```

---

# Attack #3 — Buffer Overflow

Dangerous:

```c
char path[64];

strcpy(path, user_input);
```

Safe:

```c
snprintf(path,
         sizeof(path),
         "%s",
         user_input);
```

---

# Attack #4 — Invalid HTTP Methods

Attacker:

```http
DELETE /
```

or

```http
GARBAGE /
```

Allow only:

```text
GET
```

for now.

---

Example

```c
if(strcmp(method, "GET") != 0)
{
    return;
}
```

---

# Thread Pool Server Skeleton

---

## Queue

```c
#define QUEUE_SIZE 100

int queue[QUEUE_SIZE];

int head = 0;
int tail = 0;
```

---

## Mutex

```c
pthread_mutex_t queue_lock;
```

Protects queue.

---

## Worker

```c
void* worker(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&queue_lock);

        if(head != tail)
        {
            int client = queue[head];

            head = (head + 1) % QUEUE_SIZE;

            pthread_mutex_unlock(&queue_lock);

            handle_request(client);
        }
        else
        {
            pthread_mutex_unlock(&queue_lock);
        }
    }
}
```

---

## Main Thread

Accepts clients.

Adds to queue.

```c
pthread_mutex_lock(&queue_lock);

queue[tail] = client_fd;

tail = (tail + 1) % QUEUE_SIZE;

pthread_mutex_unlock(&queue_lock);
```

---

# Why This Is Better

Day 16:

```text
1000 clients
↓
1000 threads
```

Day 17:

```text
1000 clients
↓
4 workers
```

Huge difference.

---

# Mental Model

```text
Day 12
fork()
↓

Day 16
thread per connection
↓

Day 17
thread pool
↓

Day 18+
epoll + thread pool
(production architecture)
```

---

# Day 17 Completion Checklist

You should now understand:

✅ Why thread-per-client doesn't scale

✅ What a thread pool is

✅ Why work queues exist

✅ Why mutexes protect queues

✅ Path traversal attacks

✅ Buffer overflow prevention

✅ Request size limits

✅ HTTP method validation

✅ Production server architecture basics

---
