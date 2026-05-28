# Day 12 Questions & Answers

---

# 1. Why does the parent continue looping?

The parent must remain available to:
- accept new clients
- create more child processes

without blocking.

---

# 2. Why does the child close(server_fd)?

The child should not accept new connections.

Keeping unnecessary FDs open wastes resources.

---

# 3. Why is fork() expensive?

fork() creates:
- process structures
- scheduling overhead
- virtual memory mappings

Large systems can overload quickly.

---

# 4. What is Copy-On-Write?

Linux delays memory copying after fork().

Memory pages are shared until modified.

This improves performance.

---

# 5. What is a zombie process?

A terminated child process that still exists in:
- process table
- kernel metadata

until parent reads exit status.

---

# 6. Why is waitpid() important?

Without cleanup:
- zombies accumulate
- PID table fills
- system resources leak

---

# 7. Why is process isolation useful?

Each child has:
- separate memory
- separate stack
- separate execution flow

One crash does not directly corrupt others.

---

# 8. Why did modern servers move away from fork()?

At high scale:
- too much RAM
- too many context switches
- poor scalability

Event-driven models became faster.
