# Day 11 — TCP/IP Sockets & Networking

---

# Core Concepts

A socket is a file descriptor managed by the kernel.

Sockets allow processes to communicate over:
- networks
- localhost
- TCP/IP

---

# Server Flow

```text
socket()
bind()
listen()
accept()
read()
send()
close()
```

---

# Client Flow

```text
socket()
connect()
send()
read()
close()
```

---

# Important APIs

## socket()

Creates networking endpoint.

```c
socket(AF_INET, SOCK_STREAM, 0);
```

| Parameter | Meaning |
|---|---|
| AF_INET | IPv4 |
| SOCK_STREAM | TCP |
| 0 | default protocol |

---

## bind()

Associates socket with IP + port.

---

## listen()

Marks socket as passive server socket.

---

## accept()

Creates NEW socket for connected client.

IMPORTANT:

```text
server_fd != client_fd
```

---

## connect()

Initiates TCP connection.

---

# TCP Handshake

```text
Client → SYN
Server → SYN-ACK
Client → ACK
```

---

# Important Structures

```c
struct sockaddr_in
```

Contains:
- IP address
- port
- protocol family

---

# htons()

Converts host byte order → network byte order.

---

# INADDR_ANY

Listen on all interfaces.

---

# Mental Model

```text
Application
   ↓
Socket syscall
   ↓
Kernel TCP/IP stack
   ↓
Network
```

---

# Key Takeaways

✔ sockets are file descriptors  
✔ TCP is connection-oriented  
✔ server waits for connections  
✔ clients initiate connections  
✔ kernel manages networking internals
