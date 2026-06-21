# Day 16 Questions

## 1. Why use threads instead of fork()?

Threads are lighter and share memory, making them faster for concurrent requests.

---

## 2. What is pthread_detach()?

It frees thread resources automatically when the thread exits.

---

## 3. What is the risk of shared memory?

Race conditions when multiple threads modify shared data.

---

## 4. Why pass malloc'd client_fd?

Each thread needs its own copy of the socket descriptor.

---

## 5. Why is this closer to real servers?

Because real servers use:
- threads
- event loops
- or hybrid models for concurrency
