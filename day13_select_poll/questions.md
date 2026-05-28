# Day 13 Questions & Answers

---

# 1. Why is select() better than fork()?

fork():
- creates many processes
- expensive context switching

select():
- one process
- many clients

More scalable.

---

# 2. Why must read_set = master_set?

select() modifies fd_set internally.

Original set must remain preserved.

---

# 3. What does FD_ISSET() check?

Checks whether:
- file descriptor has activity

after select() returns.

---

# 4. Why does select() become slow at scale?

Kernel repeatedly scans:
- every file descriptor

even inactive ones.

---

# 5. What does poll() improve?

poll():
- removes FD limits
- simplifies management
- improves scalability

---

# 6. What is event-driven architecture?

Program reacts only when:
- events occur

instead of blocking per client.

---

# 7. Why is one event loop powerful?

Single-threaded event loops:
- reduce synchronization
- reduce overhead
- simplify scaling

---

# 8. What systems use event-driven networking?

- Nginx
- Redis
- HAProxy
- Node.js internals
