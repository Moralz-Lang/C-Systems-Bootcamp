## 📁 File Routing

```c
/ → www/index.html
/about.html → www/about.html
```

---

## 🔐 Basic Security

* Blocks `..` path traversal
* Only allows GET requests
* Limits buffer size

---

## 🧾 MIME Types

Server detects file type:

* .html → text/html
* .css → text/css
* .js → application/javascript
* .png → image/png

---

## ⚙️ Compilation

```bash
gcc -Wall -Wextra -g day17_threadpool.c -o day17_server -pthread
```

---

## ▶ Run

```bash
./day17_server
```

---

## 🧪 Test

```bash
curl http://127.0.0.1:8080
curl http://127.0.0.1:8080/about.html
curl http://127.0.0.1:8080/style.css
```

````
