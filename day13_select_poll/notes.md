# Day 13 — Event-Driven Networking (select/poll)

---

# Why Day 12 Was Not Enough

fork() creates:
- high memory overhead
- expensive context switching
- poor scalability

Modern systems need:
- many clients
- fewer processes
- efficient event handling

---

# Event-Driven Architecture

Instead of:
- one process per client

We use:
- one process watching many sockets

---

# select()

Monitors multiple file descriptors.

Kernel blocks until:
- socket becomes readable
- writable
- exception occurs

---

# fd_set

Bitmask representing file descriptors.

Important macros:

```c
FD_ZERO()
FD_SET()
FD_ISSET()
FD_CLR()
```

---

# select() Flow

```text
register sockets
      ↓
select() sleeps
      ↓
kernel wakes process
      ↓
active sockets processed
```

---

# Why select() Is Slow

select() scans:
- all file descriptors
- every loop

Even inactive sockets are checked.

Complexity:

```text
O(n)
```

---

# poll()

Improves:
- FD management
- scalability
- flexibility

Uses:

```c
struct pollfd
```

instead of bitmasks.

---

# pollfd Structure

```c
struct pollfd
{
    int fd;
    short events;
    short revents;
};
```

---

# POLLIN

Means:
- socket ready to read

---

# Event-Driven Benefits

✔ fewer processes  
✔ lower RAM usage  
✔ better scalability  
✔ centralized event loop  

---

# Real World Usage

Architectures similar to:
- Nginx
- Redis
- proxies
- multiplayer servers

---

# Key Takeaways

✔ event-driven networking  
✔ FD multiplexing  
✔ select() architecture  
✔ poll() improvements  
✔ scalable server foundations
