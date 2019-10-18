# Tinker Kernel
Most courses reveal one layer of abstraction.

Digital Design
Micro Processor Interfacing
Computer Architecture
<---something is missing here--->
Data Structure and Algorithms

It's the userspace. Userspace is a huge collection of libraries that make our lives much easier and allow us to write programs at a higher level of abstraction. Userspace has been built over many years but there is very little awareness of it. In this project, I try to expose some the heavy lifting the userspace does for us.

## A primer
Kernel space represents the resources (memory, processes) used by the kernel, a device driver or. The kernel space represents an elevated privilege as the program is directly interacting with the hardware. It also means kernel space has a very limited set of functions to keep it lean and stable.

User space on the other hand is the whole wide world where anything works and anything goes. Most of the programming you will do in all your life will be in the userspace. The userspace is also provided with a much richer instruction set. As an example consider `malloc` which is actually a user space program implemented in `libc` library that you can access when you compile your program with C.

## Instructions
Ideally you should be using an OS with the latest kernel version.
```bash
gcc request_memory.c
ulimit -S -v 10240
./a.out
ulimit -S -v unlimited
```

```bash
gcc malloc.c
ulimit -S -v 10240
./a.out
ulimit -S -v unlimited
```

## Using the best list implementation
[linux/llist.h](https://github.com/torvalds/linux/blob/master/include/linux/llist.h)

## References
* [difference between kernel space and userspace](https://stackoverflow.com/questions/5957570/what-is-the-difference-between-the-kernel-space-and-the-user-space)
* [writing kernel drivers tutorial](http://derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction/)
* [restricting heap memory](http://geekswing.com/geek/quickie-tutorial-ulimit-soft-limits-hard-limits-soft-stack-hard-stack/)
* [ulimit man page](https://ss64.com/bash/ulimit.html)
* [limit virtual memory for program](https://unix.stackexchange.com/questions/438986/is-setting-ulimit-v-sufficient-to-avoid-memory-leak)
* [syscalls used by malloc](https://sploitfun.wordpress.com/2015/02/11/syscalls-used-by-malloc/)

## TODO
* [Explain Stack vs Heap to explain the role of malloc](https://medium.com/@nickteixeira/stack-vs-heap-whats-the-difference-and-why-should-i-care-5abc78da1a88)
* [considering overcommit](http://www.etalabs.net/overcommit.html)
* [Malloc source code](https://code.woboq.org/userspace/glibc/malloc/malloc.c.html)
* [Good malloc](http://gee.cs.oswego.edu/dl/html/malloc.html)
* [Understanding malloc](https://sploitfun.wordpress.com/2015/02/10/understanding-glibc-malloc/)
