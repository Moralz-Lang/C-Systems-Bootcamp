# Day 12 — Multi-Client Servers Using fork()

---

# Core Idea

Day 11 handled:
- one client at a time

Day 12 introduces:
- concurrent client handling

using:

```c
fork()
```

---

# Why Single-Client Servers Fail

If one client blocks:

```text
server becomes unavailable to others
```

Real servers must handle:
- multiple users
- simultaneous requests
- concurrency

---

# fork() Model

Server architecture:

```text
accept()
   ↓
fork()
   ↓
child handles client
parent continues listening
```

---

# Parent Process

Responsible for:
- accepting connections
- creating child processes
- staying alive

---

# Child Process

Responsible for:
- reading client data
- sending responses
- closing client socket

---

# Copy-On-Write (COW)

Linux optimizes fork():

```text
memory is NOT copied immediately
```

Pages are duplicated only when modified.

---

# Why close(server_fd) in Child?

Child does not need listening socket.

Only parent should accept new clients.

---

# Why close(client_fd) in Parent?

Parent does not handle client communication.

Only child uses connected client socket.

---

# Zombie Processes

When child exits:
- process remains in process table

until parent collects status.

---

# waitpid()

Used to clean zombie processes.

```c
waitpid(-1, NULL, WNOHANG);
```

---

# Architecture Type

```text
Process-per-client model
```

Used historically in:
- Apache prefork
- older UNIX servers

---

# Weaknesses

fork() model scales poorly:
- memory overhead
- process scheduling cost
- slow at large scale

---

# Key Takeaways

✔ concurrent client handling  
✔ process isolation  
✔ parent/child architecture  
✔ zombie cleanup  
✔ multi-client server model
