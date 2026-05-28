````markdown
# DAY 15 — RUN & OUTPUT GUIDE

---

# COMPILATION

```bash
gcc -Wall -Wextra -g day15_http_server.c -o http_server
````

---

# RUNNING

## Terminal 1

```bash
./http_server
```

---

## Terminal 2

```bash
curl http://127.0.0.1:8080
```

or open browser:

```text
http://127.0.0.1:8080
```

---

# EXPECTED OUTPUT

## Server

```text
HTTP server running on port 8080...
METHOD: GET | PATH: /
```

---

## Browser

Displays:

```text
Hello from C HTTP Server
This page is served from disk.
```

---

# DEBUGGING

## Trace system calls

```bash
strace ./http_server
```

Watch:

* accept
* read
* fopen
* send

---

## Check port usage

```bash
ss -tulnp
```

---

# IMPORTANT MENTAL MODEL

```text
Browser
   ↓
TCP socket
   ↓
HTTP request (text)
   ↓
C server parses request
   ↓
Filesystem lookup
   ↓
HTTP response
   ↓
Browser renders page
```
