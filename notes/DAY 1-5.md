```markdown
# C SYSTEMS BOOTCAMP — DAYS 1–5 NOTES

---

# 🟢 DAY 1 — MEMORY BASICS

## Core Concepts

### Stack
- Stores local variables  
- Automatically managed  
- Grows downward (high → low memory)

### Heap
- Dynamically allocated memory (`malloc`)  
- Must be manually freed (`free`)  
- Grows upward  

### Process Memory Layout

```

## HIGH MEMORY

## Stack

## Heap

BSS
Data
Text
----

LOW MEMORY

````

### Key Code Concept

```c
int *p = malloc(sizeof(int));
*p = 555;
free(p);
````

## Questions & Answers

**Q1: If `malloc` is not freed, what happens?**
**Answer:** Memory leak. Memory remains allocated and unavailable.

**Q2: When does OS reclaim memory?**
**Answer:** When the process terminates.

**Q3: What happens in long-running servers?**
**Answer:** Memory leaks accumulate → eventual crash or slowdown.

**Q4: What happens in embedded systems?**
**Answer:** Extremely dangerous — limited RAM (e.g., 2KB) → system failure.

## Key Insight

* Memory must be explicitly managed
* Leaks = resource loss
* Use-after-free = corruption

---

# 🟢 DAY 2 — STACK & POINTERS

## Core Concepts

### Pointer Arithmetic

* `p + 1 == p + sizeof(type)`

### Arrays Decay to Pointers

```c
int arr[3];
int *p = arr;
```

### Stack Frame Layout

```
Return Address
Saved RBP
Local Variables
```

### Buffer Overflow

```c
int arr[3];
arr[3] = 999; // UB
```

### What Happens?

* Writes beyond allocated memory
* May overwrite:

  * Saved RBP
  * Return address

## Questions & Answers

**Q1: Why does stack grow downward?**
**Answer:** Convention — allows heap and stack to grow toward each other.

**Q2: Why is use-after-free worse than a leak?**
**Answer:** It can corrupt memory and hijack execution.

**Q3: What is undefined behavior?**
**Answer:** Behavior not defined by the C standard — compiler can do anything.

## Key Insight

* C does no bounds checking
* Memory is raw bytes
* Pointer arithmetic must be precise

---

# 🟢 DAY 3 — HEAP INTERNALS

## Core Concepts

### malloc Behavior

* `malloc` returns pointer to user memory
* Metadata exists before the pointer

### Heap Layout (Simplified)

```
[ metadata ]
[ user data ] ← pointer returned here
```

### Use-After-Free

```c
free(p);
printf("%d", *p); // UB
```

### Why You Saw Random Value

* Allocator reused memory
* Metadata overwrote your value

## Questions & Answers

**Q1: Why does `(size_t*)p - 1` access metadata?**
**Answer:** Moves pointer back 8 bytes to metadata region.

**Q2: Why doesn’t `free` zero memory?**
**Answer:** Performance — unnecessary overhead.

**Q3: Why can `malloc` return same address?**
**Answer:** Freed memory is reused (free list / tcache).

## Key Insight

* Heap memory is recycled
* You do NOT own memory after `free`

---

# 🟢 DAY 4 — MEMORY SAFETY & UB

## The 5 Critical Bugs

* Buffer overflow
* Use-after-free
* Double free
* Dangling pointer
* Memory leak

## Undefined Behavior (UB)

```c
arr[10] = 5;
```

* Compiler assumes this never happens
* May optimize unpredictably

## Defensive Programming

* Initialize variables
* Check `malloc`
* Set pointer to `NULL` after `free`
* Track ownership

## Questions & Answers

**Q1: Why is a dangling pointer dangerous?**
**Answer:** Points to invalid memory that may be reused.

**Q2: Why is `free(NULL)` safe?**
**Answer:** Defined to do nothing — simplifies code safety.

**Q3: Why must every `malloc` have one `free`?**
**Answer:** Prevent leaks and double free errors.

**Q4: Why does UB behave differently under `-O2`?**
**Answer:** Compiler optimizes assuming UB never occurs.

**Q5: What do sanitizers do?**
**Answer:** Detect memory errors at runtime.

## Key Insight

* UB = no guarantees
* Safety must be enforced manually

---

# 🟢 DAY 5 — LINUX SYSCALLS & FILE DESCRIPTORS

## Core Concepts

### System Calls

* Interact with the kernel

### File Descriptors (FDs)

| FD | Meaning |
| -- | ------- |
| 0  | stdin   |
| 1  | stdout  |
| 2  | stderr  |

### Syscall Example

```c
int fd = open("file.txt", O_CREAT | O_WRONLY, 0644);
write(fd, "Hello", 5);
close(fd);
```

### User vs Kernel Space

* **User space** → your program
* **Kernel space** → OS control

Syscalls switch between them.

## Questions & Answers

**Q1: System call vs library call?**
**Answer:** Syscall → kernel interaction; library → wrapper.

**Q2: Why check return values?**
**Answer:** Syscalls can fail.

**Q3: What happens if you don’t close FD?**
**Answer:** Resource leak (like memory leak).

**Q4: How is FD reuse similar to `malloc`?**
**Answer:** Both reuse released resources.

**Q5: Why can’t user access kernel memory?**
**Answer:** Protected by CPU for system stability.

## Key Insight

* File descriptors are like pointers to kernel resources
* Must be managed carefully

---

# 🧠 FINAL UNDERSTANDING

You now understand:

* Memory layout (stack vs heap)
* Pointer arithmetic
* Heap internals
* Undefined behavior
* Memory safety
* Linux syscalls

```
```
