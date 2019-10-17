#include <stdlib.h>
#include <stdio.h>

typedef enum operation {
    DONT_FREE,
    FREE,
    NUM_OPS
} operation;

#define ALLOCATE_ITEMS 3
int allocate_size[] = {
    4,  // INT
    8,  // LONG
    16  // LONGLONG
};

int main() {

    // seed random number generator
    srand(10);

    // declare loop variables
    int bytes, count = 0;
    void* ptr = NULL;
    operation option;

    // keep allocating memory until it fails
    while (1) {

        // randomly choose bytes to allocate
        bytes = allocate_size[rand()%ALLOCATE_ITEMS];
        ptr = malloc(bytes);
        if (ptr == NULL) {
            break;
        }

        // randomly choose to free the memory or not
        // Note: memory leak is in accordance with purpose of the program
        option = rand() % NUM_OPS;
        if (option == FREE) {
            free(ptr);
        }
        count++;
    }

    printf("successful calls: %d\n", count);
    return 0;
}
