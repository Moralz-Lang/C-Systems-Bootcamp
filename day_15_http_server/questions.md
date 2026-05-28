# Day 15 Questions & Answers

---

# 1. What is HTTP in simple terms?

HTTP is a text protocol used to communicate between browsers and servers.

---

# 2. Why do we parse method and path?

To understand:
- what the client wants
- which resource to serve

---

# 3. Why do we use fopen() in a web server?

To read static files from disk and send them over HTTP.

---

# 4. Why is Content-Length important?

It tells the browser:
- how many bytes to expect

---

# 5. What happens if file is not found?

Server returns:

```text
404 Not Found
```

---

# 6. Why is HTTP stateless?

Each request is independent:
- no memory of previous requests

---

# 7. What is the key transformation in Day 15?

You move from:

```text
socket server → real web server
```

---

# 8. Why is this important?

Because all web backends (Node.js, Go, Nginx) work on this same model internally.
