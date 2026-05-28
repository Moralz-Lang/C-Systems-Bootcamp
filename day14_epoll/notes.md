# Day 14 — epoll() High-Performance Networking

---

# Why select() and poll() Are Not Enough

select() and poll():
- scan every FD repeatedly
- become inefficient at scale

Complexity:

```text
O(n)
```

Modern systems need:
- tens of thousands of sockets
- low CPU overhead
- efficient kernel notifications

---

# epoll()

Linux-specific scalable event notification system.

Kernel tracks:
- active file descriptors
- readiness state

instead of rescanning all sockets.

---

# Core Idea

Instead of asking:

```text
"Which sockets are active?"
```

epoll lets kernel notify you:

```text
"These sockets became active."
```

---

# epoll Architecture

```text
Clients
   ↓
Kernel epoll subsystem
   ↓
Ready event queue
   ↓
epoll_wait()
```

---

# epoll_create1()

Creates epoll instance.

```c
int epoll_fd = epoll_create1(0);
```

Returns:
- epoll file descriptor

---

# epoll_ctl()

Registers/modifies/removes sockets.

Operations:

| Operation | Meaning |
|---|---|
| EPOLL_CTL_ADD | add FD |
| EPOLL_CTL_MOD | modify FD |
| EPOLL_CTL_DEL | remove FD |

---

# epoll_event Structure

```c
struct epoll_event
{
    uint32_t events;
    epoll_data_t data;
};
```

---

# EPOLLIN

Means:
- socket ready for reading

---

# epoll_wait()

Blocks until:
- events become ready

Returns:
- number of active events

---

# Why epoll() Scales Better

epoll():
- tracks only active sockets
- avoids full FD scans
- uses kernel event queues

Complexity approaches:

```text
O(1)
```

for many operations.

---

# Event-Driven Server Model

Single process:
- handles thousands of clients
- minimal memory overhead
- minimal context switching

---

# Real Systems Using epoll()

- Nginx
- Redis
- HAProxy
- high-frequency trading systems
- multiplayer game servers

---

# Key Takeaways

✔ scalable networking  
✔ kernel event queues  
✔ asynchronous event handling  
✔ high-performance Linux networking  
✔ modern backend architecture
