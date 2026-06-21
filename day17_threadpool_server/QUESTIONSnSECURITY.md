
# 🟡 Day 17 — Review Questions

## 1. Why use a thread pool instead of one thread per connection?

Answer:
Thread pools reuse a fixed number of threads, reducing:
- memory usage
- context switching
- CPU overhead

---

## 2. What problem does the queue solve?

Answer:
It decouples:
- connection acceptance (producer)
- request handling (consumers)

This prevents blocking the accept loop.

---

## 3. Why do we use mutexes?

Answer:
To protect shared resources:
- queue head/tail
- client_fd array

Without mutexes → race conditions.

---

## 4. What does pthread_cond_wait do?

Answer:
It puts a thread to sleep until:
- new work arrives
- condition variable is signaled

---

## 5. Why is sscanf used in HTTP parsing?

Answer:
It extracts:
- HTTP method
- URL path

from raw TCP request text.

---

## 6. What is path traversal and why is it dangerous?

Example attack:
````

GET /../../etc/passwd

```

Risk:
Accessing files outside web root.

---

## 7. Why do we use MIME types?

Answer:
To tell browsers how to interpret data:
- HTML
- CSS
- images
- scripts

---

## 8. What happens if we don't limit buffer size?

Answer:
Risk of:
- buffer overflow
- memory corruption
- remote code execution
