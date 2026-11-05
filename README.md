# Tiny-shell

A custom Unix command-line interpreter implemented in C, focusing on strict POSIX compliance, robust process lifecycle management, signal safety, and I/O stream redirection. Inspired by the architecture described in *Computer Systems: A Programmer's Perspective* (CS:APP).

---

## Core Features & POSIX Mechanisms

### 1. Process Lifecycle & Execution
* **Process Creation & Replacement:** Spawns child processes using `fork()` and replaces address spaces via `execvp()`[cite: 1].
* **Reaping & Zombie Prevention:** Asynchronous child process reaping via non-blocking `waitpid()` with `WNOHANG | WUNTRACED` within dedicated signal handlers.
* **Signal Masking & Race Conditions:** Critical data structures (process/job tables) are protected against race conditions by temporarily masking `SIGCHLD`, `SIGINT`, and `SIGTSTP` using `sigprocmask()`[cite: 1].

### 2. Multi-Stage Pipelines (`|`)
* **Dynamic Pipeline Routing:** Arbitrary-length process chaining (`cmd1 | cmd2 | ... | cmdN`) via sequential `pipe()` descriptor allocations[cite: 1].
* **Descriptor Duplication:** Rebinding standard streams (`STDIN_FILENO`, `STDOUT_FILENO`) in children using `dup2()`[cite: 1].
* **Strict EOF Propagation:** Closes unused pipe write/read ends in both parent and child processes to prevent hanging consumers waiting indefinitely for `EOF`[cite: 1].

### 3. File Redirections (`<`, `>`, `>>`)
* **Input Redirection (`<`):** Maps target file descriptors to standard input (`STDIN_FILENO`) via `open()` with `O_RDONLY` and `dup2()`[cite: 1, 3].
* **Output Truncate (`>`):** Diverts `STDOUT_FILENO` to a file with `O_WRONLY | O_CREAT | O_TRUNC` (permissions `0644`)[cite: 1, 3].
* **Output Append (`>>`):** Appends standard output to target files via `O_WRONLY | O_CREAT | O_APPEND`[cite: 1, 3].

### 4. Job Control & Terminal Ownership
* **Process Group Isolation:** Isolates foreground and background jobs into distinct process groups using `setpgid()` to route keyboard signals deterministically[cite: 1].
* **Terminal Foreground Transfer:** Grants explicit control of the controlling terminal to active foreground groups via `tcsetpgrp()`[cite: 1].
* **Built-in Commands:** In-process execution of state-altering and inspection commands:
  * `cd` (via `chdir()`)[cite: 1]
  * `pwd` (via `getcwd()`)[cite: 1]
  * `jobs`, `fg`, `bg` (terminal state control and `SIGCONT` forwarding)[cite: 1]
  * `exit`[cite: 1]

---

## Architecture Overview

```text
User Input
    │
    ▼
[ parser.c ]  ── tokenize & parse into struct pipeline / single_cmd
    │
    ▼
[ eval.c ]    ── evaluate()
    ├── Built-in dispatch (cd, pwd, exit, jobs, fg, bg)
    └── External pipeline loop:
          ├── pipe() allocation & descriptor tracking
          ├── fork()
          │     ├── Child:  setpgid() -> dup2() -> setup_redirections() -> execvp()
          │     └── Parent: setpgid() -> track PIDs -> close unneeded FDs
          └── Foreground wait: tcsetpgrp() & waitfg()
