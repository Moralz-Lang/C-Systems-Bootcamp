# 🟢 DAY 17 LAB 3 — MIME TYPES & CONTENT DETECTION

Right now your server sends:

```http id="http1"
Content-Type: text/html
```

for everything.

That means:

```text id="mime1"
index.html
style.css
script.js
logo.png
```

are all claimed to be HTML.

Browsers don't like that.

---

# Why MIME Types Exist

The browser receives:

```http id="http2"
HTTP/1.1 200 OK
Content-Type: text/html
```

and decides:

```text id="mime2"
"Oh, this is HTML."
```

If you send:

```http id="http3"
Content-Type: image/png
```

the browser knows:

```text id="mime3"
"This is an image."
```

---

# Create MIME Detection Function

Add above:

```c id="funcpos"
void handle_request(...)
```

---

```c id="mimefunc"
const char* get_mime_type(const char* path)
{
    const char* ext = strrchr(path, '.');

    if(ext == NULL)
        return "text/plain";

    if(strcmp(ext, ".html") == 0)
        return "text/html";

    if(strcmp(ext, ".css") == 0)
        return "text/css";

    if(strcmp(ext, ".js") == 0)
        return "application/javascript";

    if(strcmp(ext, ".png") == 0)
        return "image/png";

    if(strcmp(ext, ".jpg") == 0)
        return "image/jpeg";

    if(strcmp(ext, ".jpeg") == 0)
        return "image/jpeg";

    return "application/octet-stream";
}
```

---

# Update Header Generation

Find:

```c id="oldheader"
sprintf(header,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %d\r\n"
        "\r\n",
        file_size);
```

Replace with:

```c id="newheader"
const char* mime =
    get_mime_type(filepath);

sprintf(header,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "\r\n",
        mime,
        file_size);
```

---

# Create CSS File

Inside:

```text id="tree1"
www/
```

Create:

## `style.css`

```css id="css1"
body
{
    background-color: #111;
    color: white;
    font-family: Arial;
}

h1
{
    color: lime;
}
```

---

# Update index.html

```html id="html1"
<!DOCTYPE html>
<html>

<head>

<title>Day 17 Server</title>

<link rel="stylesheet"
      href="/style.css">

</head>

<body>

<h1>Day 17 Thread Pool Server</h1>

<p>MIME type test.</p>

</body>

</html>
```

---

# Test

Run:

```bash id="test1"
./day17_server
```

Open:

```text id="url1"
http://127.0.0.1:8080
```

The browser should now request:

```text id="req1"
/
```

and

```text id="req2"
/style.css
```

You should see two requests logged.

---

# Watch It Happen

Add:

```c id="logmime"
printf("Serving: %s (%s)\n",
       filepath,
       mime);
```

before sending the response.

Expected:

```text id="out1"
Serving: www/index.html (text/html)

Serving: www/style.css (text/css)
```

---

# 🧠 What You Just Learned

The browser doesn't magically know file types.

The server tells it.

```text id="flow1"
Filesystem
      ↓
Extension
      ↓
MIME Type
      ↓
HTTP Header
      ↓
Browser Behavior
```

---

# Security Review (Day 17 Complete)

You now have:

### Input Validation

```text id="sec1"
GET only
```

### Path Traversal Protection

```text id="sec2"
../ blocked
```

### Buffer Protection

```text id="sec3"
%15s
%255s
```

### Request Size Limit

```text id="sec4"
MAX_REQUEST
```

### Thread Pool

```text id="sec5"
Fixed workers
```

### Condition Variables

```text id="sec6"
No CPU busy waiting
```

