-- COMPILATION
SELECT SERVER
gcc -Wall -Wextra -g day13_select_server.c -o select_server
-- POLL SERVER
gcc -Wall -Wextra -g day13_poll_server.c -o poll_server
RUNNING
-- Terminal 1
./select_server

OR

./poll_server
-- Terminal 2+
nc 127.0.0.1 8080
EXPECTED OUTPUT
-- Server
New client connected: 4
Client 4: hello
Netcat Client
hello

Echoed back from server.

-- DEBUGGING COMMANDS
Trace syscalls
strace ./select_server

-- Watch:

select
accept
read
send
View sockets
ss -tulnp
IMPORTANT MENTAL MODEL
Many clients
      ↓
Kernel monitors sockets
      ↓
select()/poll() wakes process
      ↓
server handles active sockets only
