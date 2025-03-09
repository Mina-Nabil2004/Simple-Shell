# Simple Shell in C

## Overview

This project implements a simple shell in C that supports executing system commands, handling environment variables, and managing background processes. The shell reads user input, parses commands, and executes them accordingly.

---

## Features

- **Command Execution:** Supports running built-in and external commands.
- **Variable Handling:** Implements a basic environment variable system using the `export` command.
- **Background Execution:** Allows running processes in the background using `&`.
- **File I/O Support:** Reads input from the user and handles file redirection.
- **Signal Handling:** Manages child processes using `SIGCHLD`.

---

## Prerequisites

- GCC Compiler (`gcc`)
- Linux/Unix environment (WSL recommended for Windows users)

---

## Compilation & Execution

### **Compile the shell:**

```sh
gcc SimpleShell.c -o SimpleShell  
```

### **Run the shell:**

```sh
./SimpleShell  
```

---

## Supported Commands

| Command              | Description                        |
| -------------------- | ---------------------------------- |
| `export VAR=value`   | Sets an environment variable.      |
| `echo $VAR`          | Prints the value of a variable.    |
| `cd <directory>`     | Changes the working directory.     |
| `exit`               | Exits the shell.                   |
| `command &`          | Runs a command in the background.  |
| `<external command>` | Executes any valid system command. |

---

## Background Process Handling

- Commands followed by `&` run in the background.
- The shell tracks child processes and prevents zombies using `SIGCHLD`.

---

## Potential Enhancements

- Implement input/output redirection (`>`, `<`).
- Add support for piping (`|`).
- Improve variable expansion to support complex cases.
- Implement command history and auto-completion.

---

## Author

Mina Nabil

