# Day 16 — Threaded HTTP Server

## Core Idea

Instead of:
- one request at a time (Day 15)
- or fork-heavy model (Day 12)

We now use:
- threads per request

---

## Why Threads?

Threads allow:
- shared memory
- lightweight concurrency
- faster than fork()

---

## Architecture

Client connects → main thread accepts → new thread handles request

---

## Key Function

### pthread_create()

Creates new execution flow:

- shares memory with parent
- runs independently

---

## pthread_detach()

Automatically cleans thread resources when finished.

Prevents memory leaks from join-less threads.

---

## Tradeoffs

✔ faster than processes  
✔ easier shared state  
❌ race conditions possible  
❌ needs synchronization (mutex later)

---

## Real-world usage

Used in:
- simple HTTP servers
- microservices
- background worker systems
