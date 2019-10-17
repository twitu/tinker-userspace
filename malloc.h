#ifndef MALLOC_H
#define MALLOC_H

int init_malloc(char* mem_block_algo);
void* custom_malloc(int size);
void custom_free(void* ptr);

#endif