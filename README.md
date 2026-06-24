# MiniShell

## Project Overview

MiniShell is a Unix-like shell implemented in C using POSIX system calls. It supports a small set of built-in commands, executes real system commands, handles basic I/O redirection, and maintains command history for the current session.

The project is designed to demonstrate core operating system concepts through a compact, readable shell implementation. It was developed and tested on macOS/Linux.

## Features

* Built-in commands:

  * `cd`
  * `pwd`
  * `echo`
  * `help`
  * `exit`
  * `history`
* Execute external system commands using POSIX process management.
* Command execution using:

  * `fork()`
  * `execvp()`
  * `waitpid()`
* Input, output, and error redirection:

  * `<`
  * `>`
  * `>>`
  * `2>`
  * `2>>`
* Single pipe support:

  * `command1 | command2`
* Session-based command history.
* Command parsing using tokenization.
* Build automation using Makefile.
* Clean compilation with zero warnings using:
  `gcc -Wall -Wextra`

## OS Concepts Used

- Process creation with `fork()`
- Program execution with `execvp()`
- Parent process synchronization with `waitpid()`
- File descriptor duplication with `dup2()`
- Standard input, output, and error redirection
- Basic command parsing and built-in dispatch

## Build and Run

Build the shell:

```sh
make
```

Run MiniShell:

```sh
./minishell
```

Remove build artifacts:

```sh
make clean
```

## Example Commands

```sh
pwd
echo hello world
cd ..
ls
ls > output.txt
cat < input.txt
ls | grep txt
ls missing-file 2> error.txt
history
exit
```

## Current Limitations

- Command history is stored only for the current session.
- Advanced shell features such as `!!`, `!n`, aliases, and shell variables are not supported.
- Quoted string parsing is limited.

## Future Improvements

- Improve command parsing for quoted strings and escaped characters.
- Add optional persistent history.
