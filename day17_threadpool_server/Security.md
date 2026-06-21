# 📁 `SECURITY.md`

```md
# 🔐 Day 17 — Security Model

## Threat Model

This server is exposed to:
- malicious HTTP requests
- malformed input
- path traversal attacks
- oversized payloads

---

## ❌ Threat 1: Path Traversal

Attack:
```

GET /../../etc/passwd

````

Mitigation:
```c
if (strstr(path, ".."))
    reject();
````

---

## ❌ Threat 2: Method Abuse

Only allow:

* GET

Reject:

* POST
* PUT
* DELETE

---

## ❌ Threat 3: Buffer Overflow

Unsafe:

```c
strcpy()
```

Safe:

```c
snprintf()
```

and bounded sscanf:

```c
%15s %255s
```

---

## ❌ Threat 4: Request Flooding

Mitigation:

* fixed thread pool
* bounded queue
* MAX_REQUEST limit

---

## ❌ Threat 5: File Access Outside Root

Mitigation:

* prefix all paths with `www/`
* block `..`

---

## 🧠 Security Philosophy

This is a **learning server**, not production-ready.

Goal:
Understand how vulnerabilities happen, not eliminate all risk.

````
