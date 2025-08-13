UNIX-like Command Prompt 

Introduction
A lightweight, POSIX-style shell written in C++. It runs external programs, supports a few built-ins, and serves as a learn-by-building project for systems and process control.

Functionality
- Interactive prompt (REPL): read command, execute, print result.
- Launch external programs via $PATH lookup.
- Built-ins handled in-process: cd, pwd, echo, exit.
- Basic tokenization (args, whitespace; room to extend for quotes).
- Foundation for I/O redirection (>, >>, <) and pipes (|) as next steps.

Purpose
Practice core shell concepts—parsing, process creation, environment handling, and minimal UX—without heavy frameworks. It’s meant as a stepping stone toward a more comprehensive shell and a portfolio piece that demonstrates comfort with low-level OS primitives.

How it’s implemented (brief)
1) REPL: loop reading a line from stdin, skip empty/whitespace lines.
2) Tokenize: split into argv tokens; detect built-in vs external.
3) Built-ins: execute directly in the parent process (e.g., change cwd for cd).
4) External commands: resolve via $PATH, create a child process, exec the program, and wait in the parent for completion; propagate exit status.
5) (Optional next steps) Redirection: open files and dup2 to stdin/stdout/stderr before exec. Pipes: create pipe(), fork producer/consumer, wire fds with dup2, close unused ends, then wait.
