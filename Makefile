regular:
	gcc -o tinker.out consume_memory.c tmalloc.c

tinker:
	gcc -D TINKER -o tinker.out consume_memory.c tmalloc.c

debug_regular:
	gcc -g -o debug.out consume_memory.c tmalloc.c

debug_tinker:
	gcc -g -D TINKER -o debug.out consume_memory.c tmalloc.c

run:
	ulimit -S -v 10240; ./tinker.out 10

run_debug:
	ulimit -S -v 10240; ./debug.out 10
