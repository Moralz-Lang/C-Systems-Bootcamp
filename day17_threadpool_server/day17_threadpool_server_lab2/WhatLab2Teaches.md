# 🟢 DAY 17 LAB 2 — SERVE REAL FILES FROM DISK

Right now the server always returns:

```text
Hello from Day 17 Thread Pool Server
```

regardless of URL.

That's not a real web server.

Today we'll upgrade:

```text
GET /
```

↓

```text
www/index.html
```

and

```text
GET /about.html
```

↓

```text
www/about.html
```

---

# Goal

Transform:

```text
HTTP Request
      ↓
URL Path
      ↓
Filesystem Lookup
      ↓
File Contents
      ↓
HTTP Response
```

---

# Create Files

## `www/index.html`

```html
<!DOCTYPE html>
<html>
<head>
    <title>Home</title>
</head>
<body>

<h1>Day 17 Home Page</h1>

<p>Served from disk.</p>

</body>
</html>
```

---

## `www/about.html`

```html
<!DOCTYPE html>
<html>
<head>
    <title>About</title>
</head>
<body>

<h1>About Page</h1>

<p>This page is loaded from the filesystem.</p>

</body>
</html>
```

---

# Replace Response Section

Find:

```c
char response[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "\r\n"
    "Hello from Day 17 Thread Pool Server\n";

send(client_fd,
     response,
     strlen(response),
     0);
```

Delete it.

---

# Add File Routing

Insert:

```c
char filepath[512];

if(strcmp(path, "/") == 0)
{
    strcpy(filepath,
           "www/index.html");
}
else
{
    snprintf(filepath,
             sizeof(filepath),
             "www%s",
             path);
}
```

---

# Open Requested File

```c
FILE *file =
    fopen(filepath, "r");
```

---

# File Exists

```c
if(file)
{
    char file_buffer[8192];

    int file_size =
        fread(file_buffer,
              1,
              sizeof(file_buffer),
              file);

    fclose(file);
```

---

# Build HTTP Header

```c
char header[256];

sprintf(header,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %d\r\n"
        "\r\n",
        file_size);
```

---

# Send Header

```c
send(client_fd,
     header,
     strlen(header),
     0);
```

---

# Send File

```c
send(client_fd,
     file_buffer,
     file_size,
     0);
}
```

---

# 404 Response

```c
else
{
    char not_found[] =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "404 Not Found";

    send(client_fd,
         not_found,
         strlen(not_found),
         0);
}
```

---

# Test 1

```bash
curl http://127.0.0.1:8080
```

Should return:

```html
<h1>Day 17 Home Page</h1>
```

---

# Test 2

```bash
curl http://127.0.0.1:8080/about.html
```

Should return:

```html
<h1>About Page</h1>
```

---

# Test 3

```bash
curl http://127.0.0.1:8080/fake.html
```

Should return:

```text
404 Not Found
```

---

# 🧠 What This Lab Teaches

You are now combining:

```text
Thread Pool
     +
HTTP Parsing
     +
Filesystem Access
     +
Routing
     +
Security Validation
```
