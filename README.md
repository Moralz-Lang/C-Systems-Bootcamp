# C Systems Bootcamp (Days 1–15)

This repository contains my Interactive ChatGpt Made low-level C systems programming bootcamp.

## Topics Covered

- Memory layout (stack vs heap)
- malloc/free and heap internals
- Pointer arithmetic and stack behavior
- Buffer overflows and undefined behavior
- Memory safety and sanitizers
- Linux system calls and file descriptors

## Structure

- day1 → Memory basics
- day2 → Stack and pointers
- day3 → Heap internals
- day4 → Memory safety
- day5 → Linux syscalls

## Tools Used
gcc
gdb
AddressSanitizer
Linux (Ubuntu)

## Goals

Build deep understanding of C for:

Operating systems
Embedded systems
Low-level software
- day1 → Memory basics
- day2 → Stack and pointers
- day3 → Heap internals
- day4 → Memory safety
- day5 → Linux syscalls

## Compile Example

```bash
gcc -Wall -Wextra -g day5/day5_syscalls.c -o day5
./day5

📘 Notes

Detailed notes [Days 1–5:]

👉 [View Full Notes](./notes/DAY1-5.md)
```

# C Systems Bootcamp (Days 6-9)

- To Be Updated
---

## 🟡 Concurrency & Threads (Day 10)
- pthreads in C
- Race conditions
- Shared memory issues
- Mutex locking
- Thread synchronization
- Thread IDs and execution flow

---

## 🔵 Networking Fundamentals (Days 11–12)
- TCP/IP socket programming
- Client/server architecture
- bind(), listen(), accept(), connect()
- Process-per-client model using fork()
- Zombie processes and waitpid()
- Copy-on-write behavior in Linux

---

## 🟣 Scalable Networking (Day 13)
- select() system call
- poll() system call
- File descriptor multiplexing
- Event-driven server architecture
- Handling multiple clients in one process
- Introduction to scalable backend design

---

## 🔴 High-Performance Linux Networking (Day 14)
- epoll() (Linux-specific)
- Kernel event notification system
- Scalable socket management
- High-performance server architecture
- Comparison: select vs poll vs epoll
- Event-driven backend systems (Nginx-style model)

---

## ⚫ Application Layer: HTTP Server (Day 15)
- Building a real HTTP server in C
- Parsing HTTP requests manually
- GET method handling
- URL routing basics
- Serving static files from disk
- HTTP response formatting
- Content-Length and headers
- Browser ↔ server communication model

---
