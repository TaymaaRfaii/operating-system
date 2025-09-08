# Operating System Assignment 1 – xv6

## Overview
This project is part of Operating Systems course.  
it is based on **xv6-riscv**, a UNIX-like teaching operating system, and focuses on **process management** and **system calls**.

## Implemented Tasks

### Task 1 – Hello World
- Added `helloworld.c`, a user-space program that prints:

Hello World xv6

### Task 2 – `memsize` System Call
- Implemented a new system call:

```c
int memsize(void);


Returns the calling process’s memory size.

Includes a test program: memsize_test.c.
### Task 3 – Goodbye World

Extended exit and wait system calls to support exit messages.

Added goodbye.c and modified sh.c to print exit messages.

### Task 4 – Distributing Work

Implemented:

int forkn(int n, int* pids);
int waitall(int* n, int* statuses);


Created bigarray.c to distribute array-sum computation across child processes.
