# MiniShell

A lightweight Unix-like shell built in **C** using **POSIX system calls**, designed to demonstrate core Operating System concepts such as process creation, command execution, inter-process communication, file descriptor management, and command parsing.

MiniShell supports built-in commands, external command execution, I/O redirection, single-pipe communication, and session-based command history through a clean and modular implementation.

---

## Features

### Built-in Commands

- `cd` – Change the current working directory
- `pwd` – Print the current working directory
- `echo` – Display text and arguments
- `history` – Show command history for the current session
- `help` – Display available commands
- `exit` – Terminate the shell

### External Command Execution

Execute Linux/macOS system commands using POSIX process management:

- `fork()`
- `execvp()`
- `waitpid()`

### Input / Output Redirection

Supports:

```sh
<
>
>>
2>
2>>
```

Examples:

```sh
ls > output.txt
cat < input.txt
ls missing-file 2> error.txt
```

### Inter-Process Communication

Supports single-pipe execution:

```sh
command1 | command2
```

Example:

```sh
ls | grep txt
```

### Command History

- Stores commands executed during the current shell session
- Accessible using the `history` command

### Additional Highlights

- Token-based command parsing
- Built-in command dispatching
- Makefile-based build automation
- Clean compilation with:

```sh
gcc -Wall -Wextra
```

- Zero compiler warnings

---

## Operating System Concepts Demonstrated

| Concept | Implementation |
|----------|---------------|
| Process Creation | `fork()` |
| Program Execution | `execvp()` |
| Process Synchronization | `waitpid()` |
| File Descriptor Management | `dup2()` |
| Input/Output Redirection | `<`, `>`, `>>`, `2>`, `2>>` |
| Inter-Process Communication | `pipe()` |
| Command Parsing | Tokenization |
| Built-in Command Handling | Internal Shell Dispatch |

---

## Project Structure

```text
MiniShell/
├── .gitignore
├── Makefile
├── README.md
└── miniShell.c
```

---

## Build and Run

Compile MiniShell:

```sh
make
```

Run the shell:

```sh
./minishell
```

Clean build artifacts:

```sh
make clean
```

---

## Example Commands

```sh
pwd
echo Hello World
cd ..
ls
ls > output.txt
cat < input.txt
ls | grep txt
ls missing-file 2> error.txt
history
exit
```

---

## Sample Session

```text
MiniShell> pwd
/home/user/projects

MiniShell> echo Hello World
Hello World

MiniShell> ls | grep txt
notes.txt

MiniShell> history
1 pwd
2 echo Hello World
3 ls | grep txt
```

---

## Current Limitations

- Command history is stored only for the current session
- Supports only a single pipe (`|`)
- Limited handling of quoted strings and escaped characters
- No support for aliases or shell variables
- No background process execution (`&`)

---

## Future Improvements

- Persistent command history
- Multi-stage pipelines (`cmd1 | cmd2 | cmd3`)
- Background job execution
- Enhanced parsing for quoted strings and escape sequences
- Alias and environment variable support
- Tab completion and command suggestions

---

## Learning Outcomes

Through this project, the following concepts were explored and implemented:

- UNIX process lifecycle
- Shell architecture and command execution flow
- File descriptor manipulation
- Inter-process communication using pipes
- Input/output stream redirection
- System-level programming in C
- POSIX APIs and Linux/macOS command execution

---

## Tech Stack

- **Language:** C
- **Core Concepts:** Operating Systems, UNIX Internals, POSIX System Calls
- **Tools:** GCC, Makefile, Linux/macOS Terminal
