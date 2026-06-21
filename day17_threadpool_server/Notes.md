# 🟢 Day 17 — Thread Pool HTTP Server (C Systems Bootcamp)

## Overview
Day 17 focuses on building a scalable HTTP server using:
- pthreads (thread pool model)
- producer/consumer queue
- socket programming
- HTTP parsing
- filesystem-based routing
- basic security validation
- MIME type handling

---

## 🧠 Core Architecture

Instead of spawning a thread per request:

Old model:
Client → thread → destroy thread ❌

New model:
Client → queue → fixed worker threads → response ✅

---

## 🧵 Thread Pool Design

We create:
- 4 persistent worker threads
- a shared queue of client sockets
- mutex + condition variable synchronization

Workers continuously:
1. Wait for jobs
2. Pull from queue
3. Process HTTP request

---

## 📦 Request Flow

1. accept() receives client
2. client_fd pushed into queue
3. worker thread wakes up
4. handle_request() runs
5. file is read from disk
6. HTTP response sent

---

## 🌐 HTTP Handling

We parse:

```c
sscanf(buffer, "%s %s", method, path);
