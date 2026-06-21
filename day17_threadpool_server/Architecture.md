# 📁 `ARCHITECTURE.md`

```md
# 🏗️ Day 17 — System Architecture

## High-Level Design

````

Client
↓
accept()
↓
Queue (Producer)
↓
Thread Pool (Consumers)
↓
handle_request()
↓
Filesystem
↓
HTTP Response

````

---

## 🧵 Thread Pool Model

- Fixed number of threads
- Each thread loops forever
- Waits on condition variable
- Processes queued sockets

---

## 📦 Queue System

Circular buffer:

```c
int queue[QUEUE_SIZE];
head
tail
````

Operations:

* enqueue (main thread)
* dequeue (worker threads)

---

## ⚙ Synchronization

### Mutex

Protects queue integrity

### Condition Variable

Prevents busy waiting

---

## 🌐 Request Lifecycle

1. TCP connection accepted
2. FD pushed into queue
3. Worker wakes
4. Reads HTTP request
5. Parses method/path
6. Loads file from disk
7. Sends HTTP response
8. Closes connection

---

## 📊 Performance Model

Thread-per-request:

* ❌ expensive
* ❌ unbounded growth

Thread pool:

* ✅ stable memory usage
* ✅ predictable performance

---

## 🚀 Scaling Path

Day 17 →
Thread Pool Server

Next step →
Day 18:
epoll + thread pool hybrid

Then →
Event-driven architecture (nginx-style)

````
