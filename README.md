# Tinker user space
In tinker-user-space you explore user space libraries you use, abuse and take for granted. These libraries perform a lot of the heavy lifting bridge the gap between sophisticated kernel calls and the clean user-friendly APIs you use. Try to implement simple versions of these libraries and in the process learn something about code, os, computers and life.


## User space no kernel space no user space

> **You:** Just what the hell is this, I came here to write code?

**Tinker:** User code runs in user space. Kernel code runs in kernel space.

> **You:** That didn't help.

**Tinker:** I know. So let's clear up something. Linux is the kernel and Ubuntu is the operating system (OS). The kernel contains code that interacts directly with the hardware like when you plugin your keyboard kernel code detects the device. Most of the applications that come installed with your application run in the user space. Nautilus your file browser, Firefox your web browser, Vim your text editor and your cracked game all run in user space.

> **You:** Where did space come into running code?

**Tinker:** Here space refers to memory. User space is the region of memory where user code executes. It also refers to the privilege level of the code where kernel space has more privilege.

> **You:** So code means process, so a process runs in one space or the other?

**Tinker:** Not exactly. the same process can switch between kernel mode and user mode depending on the code it is running. For e.g. `printf` is [sophisticated wrapper](https://oded.blog/2017/05/24/printf/) around the `write` kernel call. When you call `printf` in user space it calls `write` kernel call internally. `write` executes in kernel space, does whatever it needs to and then returns to user space.

> **You:** Higher priviledges kernel code thinks it's special?

**Tinker:** Kernel code interacts with hardware and it wants to feel secure that it is doing something important, because it is. Priviledges are way to make sure other process can't meddle with its operations. Try this `ps -lp 1`, it's the systemd process which is the first process to startup. Now try `ps -o user= -p 1`, see systemd executes in root to perform [its duties](https://www.linode.com/docs/quick-answers/linux-essentials/what-is-systemd/).

> **You:** Awesome so I can use `sudo` to change kernel code?

**Tinker:** Finally you get it, try `sudo rm -rf /`. *DONT*. Super User DO (`sudo`) means you can run commands with escalated privileges, to modify settings or install packages etc. To change kernel code you can [hot-load your own module](http://derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction/) into the kernel or modify the source code of the kernel and [compile it yourself](https://www.linux.com/tutorials/how-compile-linux-kernel-0/).

> **You:** Mmmmm, I want more

**Tinker:** [link](https://unix.stackexchange.com/questions/87625/what-is-difference-between-user-space-and-kernel-space)

> **You:** Not enough

**Tinker:** [link](https://www.redhat.com/en/blog/architecting-containers-part-1-why-understanding-user-space-vs-kernel-space-matters)


## Exploring `malloc`

**Tinker:** Now that you are primed let's dig into `malloc`. You use it to allocate a block of memory. But do you know how it works, what goes on behind the scenes?

> **You:** Psshh, I know what `malloc` is it's a system call. I pass it the number of bytes and it gives me a pointer to the memory block. And let me tell you something else. I know `free` as well. I give it a block of memory and it gives it back to OS. [link](https://linux.die.net/man/3/malloc).


**Tinker:** Ummm actually `malloc` is [not a system call](https://www.humblec.com/who-told-malloc-is-a-system-call/) and `free` [doesn't always return memory](https://stackoverflow.com/questions/45538993/why-dont-memory-allocators-actively-return-freed-memory-to-the-os) to the OS.

> **You:** Say what?!

**Tinker:** So `malloc` just like `printf` is actually a wrapper on the `sbrk` and `mmap` system call (depending on the usage). `malloc` is just another userspace code implemented in the glibc library that you can use when you `#include <stdlib.h>`. System calls should be used sparingly, so malloc does a lot of in-house memory management so when you `free` mem ....

> **You:** Whoooaaa `malloc` manages memory. But.. doesn't that mean it will need data structures and algorithms and take a lot of time and consume a lot of memory.

**Tinker:** ... Exactly and that's what you'll explore by trying to implement something like `malloc`. As I was saying `free` actually returns memory to the `malloc` code. `malloc` adds this to its list of unused memory and very rarely it actually returns memory to the OS.

## Finally the code

> `tmalloc.c`

This your wannabe `malloc` implementation. It has three public APIs namely,
* `init_tmalloc` to choose the algorithms
* `tmalloc` shows same behaviour as `malloc`
* `tfree` shows same behaviour as `free`

> `consume_memory.c`

This is a user program. It asks for memory uses it and frees it. A random number generator (RNG) is used to decide whether to allocate or free memory. The RNG also decides the amount of memory to allocate. Your `tmalloc` will be benchmarked on this program.


## Instructions
The instructions are for Linux based operating systems only. I am not sure if the code can be ported to other OS but PRs are welcome.

```bash
make regular
make run
make tinker
make run
```

`make regular` compiles the program with the glibc implementation of `malloc`. `make tinker` compiles the program with your implementation of `tmalloc`.

## Measure performance
The `consume_memory.c` script behaves like a user program. It asks for memory and frees it. A random number generator (RNG) is used to decide whether to allocate or free memory. The RNG also decides the amount of memory to allocate.

Just like `malloc`, `tmalloc` fails when it cannot successfully allocate the requested amount of memory. This can happen when there is no continous free block of the requested size or the program has reached memory limit. On failure `tmalloc` returns `NULL`. The number of operations performed before `tmalloc` fails is used as a measure of how well it is managing the memory.

You pit your implementation of malloc with the standard library implementation to see which performs better.

## Using the best list implementation
[linux/llist.h](https://github.com/torvalds/linux/blob/master/include/linux/llist.h)

## References
* [difference between kernel space and userspace](https://stackoverflow.com/questions/5957570/what-is-the-difference-between-the-kernel-space-and-the-user-space)
* [restricting heap memory](http://geekswing.com/geek/quickie-tutorial-ulimit-soft-limits-hard-limits-soft-stack-hard-stack/)
* [ulimit man page](https://ss64.com/bash/ulimit.html)
* [limit virtual memory for program](https://unix.stackexchange.com/questions/438986/is-setting-ulimit-v-sufficient-to-avoid-memory-leak)
* [syscalls used by malloc](https://sploitfun.wordpress.com/2015/02/11/syscalls-used-by-malloc/)
* [using ulimit in makefile](https://stackoverflow.com/questions/17547625/how-to-use-shell-builtin-function-from-a-makefile)

## TODO
* [Explain Stack vs Heap to explain the role of malloc](https://medium.com/@nickteixeira/stack-vs-heap-whats-the-difference-and-why-should-i-care-5abc78da1a88)
* [considering overcommit](http://www.etalabs.net/overcommit.html)
* [Malloc source code](https://code.woboq.org/userspace/glibc/malloc/malloc.c.html)
* [Good malloc](http://gee.cs.oswego.edu/dl/html/malloc.html)
* [Understanding malloc](https://sploitfun.wordpress.com/2015/02/10/understanding-glibc-malloc/)
* [Understanding sbrk](https://www.usna.edu/Users/cs/aviv/classes/ic221/s16/lec/11/lec.html)
