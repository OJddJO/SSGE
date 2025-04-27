#include "memtrack.h"

#if DEBUG
    size_t MEMT_mem_usage = 0;
    uint64_t MEMT_mem_allocation = 0;
    uint64_t MEMT_mem_free = 0;

    void *MEMT_malloc(size_t size) {
        void *ptr = malloc(size);
        if (ptr == NULL) {
            fprintf(stderr, "Failed to allocate memory\n");
        }
        printf("Allocated %zu memory at %p\n", size, ptr);
        ++MEMT_mem_allocation;
        return ptr;
    }

    void MEMT_free(void *ptr) {
        printf("Freed memory at %p\n", ptr);
        free(ptr);
        ++MEMT_mem_free;
    }

    void MEMT_print_mem_usage() {
        printf("Memory usage: %zu\n", MEMT_mem_usage);
        printf("Memory allocation: %llu\n", MEMT_mem_allocation);
        printf("Memory free: %llu\n", MEMT_mem_free);
    }
#endif //DEBUG