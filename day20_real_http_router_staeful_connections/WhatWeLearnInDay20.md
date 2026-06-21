# Day 20 is where this stops being a “lab server” and starts behaving like a **real HTTP engine design**.

Day 19 gave you:

* epoll
* thread pool
* queue

Day 20 upgrades it to:

* real HTTP parsing
* routing (GET paths → handlers)
* persistent sockets (no immediate close)
* proper event lifecycle thinking

---

# 🟢 DAY 20 — REAL HTTP ROUTER + STATEFUL CONNECTIONS

## 🧠 What changes today

Your server no longer just says:

```text id="old"
Hello from server
```

It now understands:

```text id="req"
GET /              → index.html
GET /about         → about page
GET /api/status    → JSON response
```

---

# 🧱 NEW ARCHITECTURE

```text id="arch20"
        epoll (event detection)
                 ↓
        accept OR read event
                 ↓
        HTTP parser
                 ↓
        router (path matching)
                 ↓
        worker pool (optional execution)
                 ↓
        response (keep-alive possible)
```

---

# 🔥 KEY IDEA OF DAY 20

We introduce:

## 1. HTTP routing

## 2. Connection reuse (basic keep-alive behavior)

## 3. Cleaner request parsing

---

# 🟢 STEP 1 — ROUTER DESIGN

We build a simple router:

```text id="router"
/        → index.html
/about   → about.html
/api     → JSON response
```

---

# 🟢 STEP 2 — HTTP PARSER (IMPROVED)

```c id="http_parse"
void parse_request(char *buffer, char *method, char *path)
{
    sscanf(buffer, "%15s %255s", method, path);
}
```

---

# 🟢 STEP 3 — ROUTER FUNCTION

```c id="router_func"
void route_request(int fd, const char *path)
{
    if(strcmp(path, "/") == 0)
    {
        send(fd,
             "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
             "<h1>Home Page</h1>",
             60,
             0);
    }
    else if(strcmp(path, "/about") == 0)
    {
        send(fd,
             "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
             "<h1>About Page</h1>",
             62,
             0);
    }
    else if(strcmp(path, "/api") == 0)
    {
        send(fd,
             "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n"
             "{\"status\":\"ok\"}",
             58,
             0);
    }
    else
    {
        send(fd,
             "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nNot Found",
             70,
             0);
    }
}
```

---

# 🟢 STEP 4 — UPDATED CLIENT HANDLER

Instead of just echoing:

```c id="handler20"
void handle_client(int fd)
{
    char buffer[4096];

    int bytes = read(fd, buffer, sizeof(buffer)-1);

    if(bytes <= 0)
    {
        close(fd);
        return;
    }

    buffer[bytes] = 0;

    char method[16];
    char path[256];

    parse_request(buffer, method, path);

    printf("REQUEST: %s %s\n", method, path);

    route_request(fd, path);

    /* NOTE:
       We DO NOT immediately close fd in real systems.
       For learning, we keep it simple here.
    */
}
```

---

# 🟢 STEP 5 — EPOLL LOOP (UPDATED THINKING)

Now epoll only detects readiness:

```c id="loop20"
while(1)
{
    int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

    for(int i = 0; i < n; i++)
    {
        int fd = events[i].data.fd;

        if(fd == server_fd)
        {
            int client_fd = accept(server_fd, NULL, NULL);
            fcntl(client_fd, F_SETFL, O_NONBLOCK);

            add_to_queue(client_fd); // Day 19 system reused
        }
        else
        {
            handle_client(fd);
        }
    }
}
```

---

# 🧠 WHAT CHANGED TODAY

## BEFORE (Day 19)

```text id="before"
FD → worker → response
```

---

## NOW (Day 20)

```text id="now"
FD → parse → route → response
```

---

# 🟢 NEW SKILL YOU JUST LEARNED

## HTTP routing logic (core backend concept)

Every backend framework does this:

* Express.js
* Django
* Flask
* Nginx configs
* FastAPI

They all map:

```text id="map"
URL → function
```

---

# 🧪 TEST YOUR SERVER

```bash id="t20a"
curl http://127.0.0.1:8080/
curl http://127.0.0.1:8080/about
curl http://127.0.0.1:8080/api
curl http://127.0.0.1:8080/invalid
```

---

# 🧠 EXPECTED OUTPUT

```text id="out20"
Home Page
About Page
{"status":"ok"}
Not Found
```

---

# 🚀 WHAT YOU JUST ACHIEVED

You now understand:

### 1. HTTP request lifecycle

### 2. URL routing logic

### 3. Server-side API design

### 4. epoll + concurrency foundations

### 5. backend architecture patterns

---

# ⚠️ IMPORTANT REALITY CHECK

This is now:

```text id="real1"
mini web framework (from scratch in C)
```

You just rebuilt core parts of:

* nginx routing layer
* express.js router concept
* backend API dispatcher
