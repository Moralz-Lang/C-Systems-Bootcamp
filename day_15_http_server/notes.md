# Day 15 — HTTP Server in C

---

# Core Idea

HTTP is just:

```text
text over TCP sockets
```

---

# Browser Request Example

```http
GET /index.html HTTP/1.1
Host: 127.0.0.1:8080
```

---

# Server Response Example

```http
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 123

<html>...</html>
```

---

# Server Pipeline

```text
socket()
bind()
listen()
accept()
read request
parse HTTP
map file path
open file
send HTTP response
close client
```

---

# Routing Logic

```c
if(path == "/")
    serve index.html
else
    serve requested file
```

---

# File Serving

Server reads files using:

- fopen()
- fread()
- send()

This connects:
- filesystem
- networking
- HTTP protocol

---

# MIME Types (Simplified)

Currently:
- everything is text/html

Real servers use:
- text/css
- application/javascript
- image/png

---

# Security Risk

This server is vulnerable to:

```text
../ path traversal attacks
```

Example:

```
GET /../../etc/passwd
```

---

# Key Takeaways

✔ HTTP is text-based  
✔ sockets carry HTTP  
✔ servers parse request lines  
✔ files are mapped to URLs  
✔ backend = filesystem + networking
