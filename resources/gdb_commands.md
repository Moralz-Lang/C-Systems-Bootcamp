# GDB Commands Cheat Sheet

## Compile for debugging
```bash
gcc -g file.c -o program
--
## Start GDB
gdb ./program
Breakpoints
break main
break function_name
Run program
run
Step execution
next      # step over
step      # step into
continue  # resume execution
Inspect variables
print x
print &x
Memory inspection
x/10x addr     # hex dump
x/10d addr     # decimal
x/10i addr     # instructions
Stack inspection
bt            # backtrace
info stack
Registers
info registers

---

# 🟢 2. `gcc_flags.md`

```markdown id="gcc1"
# GCC Flags Cheat Sheet

## Basic compile
```bash
gcc file.c -o program
Debug build
gcc -g file.c -o program
Warnings (IMPORTANT)
gcc -Wall -Wextra file.c -o program
Treat warnings as errors
gcc -Wall -Wextra -Werror file.c -o program
Debug + warnings
gcc -g -Wall -Wextra file.c -o program
Threaded programs
gcc file.c -o program -pthread
Sanitizers (VERY IMPORTANT)
Memory errors
gcc -fsanitize=address file.c -o program
Undefined behavior
gcc -fsanitize=undefined file.c -o program

---

# 🟢 3. `linux_commands.md`

```markdown id="linux1"
# Linux Commands Cheat Sheet

---

## File operations
```bash
ls -l
cd directory
pwd
mkdir test
rm file
rm -r folder
File viewing
cat file
less file
head file
tail file
Process management
ps aux
top
kill PID
kill -9 PID
Permissions
chmod +x file
chmod 777 file
Networking basics
ip a
ping google.com
netstat -tulnp
Compilation tools
gcc file.c -o program
make

---

# 🟢 4. `pthread_cheatsheet.md`

```markdown id="pthread1"
# Pthread Cheat Sheet

---

## Include header
```c
#include <pthread.h>
Create thread
pthread_t t;

pthread_create(&t, NULL, function, arg);
Thread function format
void* function(void* arg)
Wait for thread
pthread_join(t, NULL);
Mutex init
pthread_mutex_t lock;
pthread_mutex_init(&lock, NULL);
Lock / Unlock
pthread_mutex_lock(&lock);
pthread_mutex_unlock(&lock);
Destroy mutex
pthread_mutex_destroy(&lock);
Compile
gcc file.c -o program -pthread

---

# 🧠 DO YOU NEED ANY MORE FILES?

Yes — but only optional upgrades.

If you want your repo to look **senior-level**, add:

---

## 🟡 OPTIONAL (BUT HIGHLY RECOMMENDED)

### 5. `memory_model.md`

Explains:

- stack vs heap diagrams
- process memory layout
- virtual memory

---

### 6. `debugging_playbook.md`

Explains:

- how to use GDB like a workflow
- how to detect bugs step-by-step
- how to analyze crashes

---

### 7. `common_bugs.md`

Explains:

- use-after-free
- buffer overflow
- race conditions
- segmentation faults
- undefined behavior
