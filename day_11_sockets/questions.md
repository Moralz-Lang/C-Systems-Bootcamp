# Day 11 Questions & Answers

---

# 1. Why is a socket considered a file descriptor?

Because Linux treats sockets like files internally.

They can use:
- read()
- write()
- close()

through the same kernel FD table.

---

# 2. Why does accept() return a new socket?

The listening socket must remain available for future clients.

The new socket represents the connected client session.

---

# 3. What does htons() do?

Converts host byte order into network byte order (big-endian).

Needed for cross-platform communication.

---

# 4. Why is TCP reliable?

TCP guarantees:
- ordering
- retransmission
- error checking
- connection state

through kernel-managed protocol logic.

---

# 5. What happens inside connect()?

TCP three-way handshake begins:

```text
SYN → SYN-ACK → ACK
```

Connection becomes established.
