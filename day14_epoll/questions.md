# Day 14 Questions & Answers

---

# 1. Why is epoll() faster than select()?

select():
- scans every FD

epoll():
- kernel tracks active FDs only

Less CPU overhead.

---

# 2. What is the purpose of epoll_wait()?

epoll_wait():
- blocks process
- returns active events only

Efficient event notification.

---

# 3. Why is epoll Linux-specific?

epoll is:
- implemented inside Linux kernel

Other OSes use alternatives:
- kqueue (BSD/macOS)
- IOCP (Windows)

---

# 4. What happens when client disconnects?

read() returns:
- 0 or negative value

Server:
- closes socket
- removes FD from epoll

---

# 5. Why is event-driven architecture scalable?

One process can:
- manage many sockets
- avoid thread/process explosion
- reduce RAM usage

---

# 6. What does EPOLL_CTL_ADD do?

Registers file descriptor with epoll instance.

Kernel begins tracking readiness events.

---

# 7. Why do modern web servers use epoll()?

epoll supports:
- massive concurrency
- low latency
- efficient scaling

Critical for production traffic.

---

# 8. What is the biggest conceptual shift with epoll()?

Application no longer:
- polls all sockets manually

Kernel pushes readiness notifications.
