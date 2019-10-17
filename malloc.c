#define _DEFAULT_SOURCE

#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "malloc.h"

#define INCREMENT 4096  // generally 4KB is the default page size

// global variables
void* curr_brk, *blocks;
void* (*mem_block_algo)(void*, int);  // function pointer to chosen algorithm
void* (*failsafe_block_algo)(void*, int);  // failsafe algo is called after requesting memory
void* (*defrag_block_algo)(void*);  // combine small blocks of memory together
int ret;

// block header structure
typedef struct block_hdr {
    int size;
    bool block_start;
    bool allocated;
    struct block_hdr* next;
} block_hdr;
typedef block_hdr* Block_hdr;

/*****************************************
 * Block selecting algorithms
 * first_fit_block_algo
 * failsafe_check_new_block
 *****************************************/

/**
 * Traverse all blocks and return first unallocated
 * block that satisfies size requirement
 */
void* first_fit_block_algo(void* blocks, int size) {

    // initialize variables to traverse blocks
    void* traverse = blocks;
    void* alloc_block = NULL;
    Block_hdr hdr;
    
    while (true) {
        if (traverse == NULL) return NULL;

        hdr = (Block_hdr) traverse;
        // checks if block has size + bytes required for next block header
        if (!hdr->allocated && hdr->size >= size + sizeof(block_hdr)) {
            alloc_block = traverse;
            break;
        }

        traverse = hdr->next;
    }

    return alloc_block;
}

/**
 * Checks if first block satisfied memory requirement
 * Note: Ideally should be called after requesting more memory
 */
void* failsafe_check_new_block(void* blocks, int size) {

    void* alloc_block = NULL;
    Block_hdr hdr = (Block_hdr) blocks;
    if (!hdr->allocated && hdr->size >= size + sizeof(block_hdr)) {
        alloc_block = blocks;
    }

    return alloc_block;
}

/**
 * Request more heap memory by incrementing program break
 */
int request_memory() {

    // increment current program break location
    curr_brk = sbrk(0);
    int ret = brk(curr_brk + INCREMENT);
    if (ret == -1) return -1;  // failed to find program break location

    // allocate new block of memory
    Block_hdr hdr = (Block_hdr) curr_brk;
    hdr->size = INCREMENT - sizeof(block_hdr);
    hdr->next = blocks;
    hdr->allocated = false;
    hdr->block_start = true;
    blocks = hdr;

    return 0;  // successfully allocated memory
}

/**
 * Allocate memory from given block. Mark block
 * as allocated and break it into a new unallocated block
 */
void* malloc_mem(void* block, int size) {

    // parse existing block
    void* empty_mem = block + sizeof(block_hdr);
    Block_hdr hdr = (Block_hdr) block;
    int empty_size = hdr->size;
    
    // create new memory block
    void* new_block = empty_mem + size;
    Block_hdr new_hdr = (Block_hdr) new_block;
    new_hdr->allocated = true;
    new_hdr->size = empty_size - size - sizeof(block_hdr);
    new_hdr->next = hdr->next;
    hdr->size -= size;

    return empty_mem;
}

/***********************************
 * PUBLIC APIS
************************************
 * init_malloc
 * malloc
 * free
************************************/

/**
 * Intializes global variables and block finding algorithm
 * if function is passed invalid function name returns with -1
 */
int init_malloc(char* algo_name) {
    // initialize global variables
    blocks = NULL;
    curr_brk = NULL;

    // set mem_block_algo memory allocation
    if (strcmp(algo_name, "first_fit_block_algo")) {
        mem_block_algo = &first_fit_block_algo;
    } else {
        return -1;
    }
}

/**
 * Tries to allocate memory of given size
 * return pointer to memory if successful else
 * returns NULL
 */
void* custom_malloc(int size) {
    
    void* alloc_block;
    alloc_block = mem_block_algo(blocks, size);

    // malloc_mem fails, request more memory from program stack
    if (alloc_block == NULL) {
        ret = request_memory();
        if (ret == 0) {  // successful block allocation
            alloc_block = failsafe_block_algo(blocks, size);
        }
    }

    if (alloc_block != NULL) {
        return malloc_mem(alloc_block, size);
    } else {
        return NULL;
    }
}

/**
 * Frees given block
 * Note: fails silently
 */
void custom_free(void* ptr) {
    
    Block_hdr ptr_hdr = (Block_hdr) (ptr - sizeof(block_hdr));
    Block_hdr traverse = ptr_hdr->next;
    ptr_hdr->allocated = false;
    
}

int main() {
    // printf("%d\n", getpid());
    printf("%d\n", getpid()); 
    init_malloc("first_fit_block_algo");
    int count = 0;
    while (true) {
        ret = request_memory();
        if (ret == -1) break;
        count++;
    }
    printf("%d\n", count);
    getchar();
}
