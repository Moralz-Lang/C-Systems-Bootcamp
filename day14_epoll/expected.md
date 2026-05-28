# Compilation

```bash
gcc -Wall -Wextra -g day14_epoll_server.c -o epoll_server
```

---

# Running

## Terminal 1

```bash
./epoll_server
```

---

## Terminal 2+

```bash
nc 127.0.0.1 8080
```

---

# Expected Output

## Server

```text
epoll server running on port 8080...
New client connected: 5
Client 5 says: hello
```

---

## Netcat Client

```text
hello
```

Echoed back from server.

---

# Debugging Commands

## Trace Syscalls

```bash
strace ./epoll_server
```

Watch these syscalls:

* epoll_create1
* epoll_ctl
* epoll_wait
* accept
* read
* send

---

## View Active Sockets

```bash
ss -tulnp
```

---

# Important Mental Model

```text
Many clients
      ↓
Kernel epoll queue
      ↓
Only active events returned
      ↓
Server processes ready sockets
```

---

# Core Understanding

Unlike `select()` and `poll()`, epoll does not repeatedly scan all file descriptors.

Instead:

* the Linux kernel tracks active sockets internally
* only ready events are returned to userspace
* the server becomes highly scalable

This architecture powers:

* Nginx
* Redis
* HAProxy
* modern backend infrastructure
