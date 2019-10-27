#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "llist.h"
#include "tmalloc.h"

// bias for allocate
typedef enum operation {
    ALLOCATE,
    MORE_ALLOCATE,
    ALLOCATE_MORE,
    FREE,
    NUM_OPS
} operation;

#define ALLOCATE_ITEMS 3
int allocate_size[] = {
    sizeof(struct llhead) + 4,  // INT
    sizeof(struct llhead) + 8,  // INT
    sizeof(struct llhead) + 16  // INT
};

int main(int argc, char* argv[]) {

    #ifdef TINKER
        init_tmalloc("first_fit_block_algo");
    #endif

    // seed random number generator
    int seed = atoi(argv[1]);
    srand(seed);

    // declare loop variables
    int bytes, count = 0;
    operation option;
    struct llhead* temp;

    // initialize list structure to store allocated memory
    LL_HEAD(llist);
    struct llhead* traverse = &llist;

    // keep allocating memory until it fails
    // i.e. malloc returns NULL
    while (true) {

        // randomly choose to free the memory or not
        // Note: memory leak is in accordance with purpose of the program
        option = rand() % NUM_OPS;
        if (option == FREE) {
            // do not free list head
            if (traverse == &llist) continue;

            temp = traverse;
            traverse = traverse->prev;
            LL_DEL(temp);

            #ifdef TINKER
                tfree(temp);
            #else
                free(temp);
            #endif

            count++;
        } else {
            
            // randomly choose bytes to allocate
            bytes = allocate_size[rand()%ALLOCATE_ITEMS];

            #ifdef TINKER
                temp = tmalloc(bytes);
            #else
                temp = malloc(bytes);
            #endif

            if (temp == NULL) break;  // break when malloc fails
            LL_ADD(&llist, temp);
            traverse = temp;
            count++;
        }
    }

    printf("successful calls: %d\n", count);
    return 0;
}
