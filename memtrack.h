#ifndef __MEMTRACK_C__
#define __MEMTRACK_C__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef DEBUG
    #define DEBUG 1
#endif // DEBUG

#if DEBUG
    extern size_t MEMT_mem_usage;
    extern uint64_t MEMT_mem_allocation;
    extern uint64_t MEMT_mem_free;

    void *MEMT_malloc(size_t size);
    void MEMT_free(void *ptr);
    void MEMT_print_mem_usage();
#else
    #define MEMT_malloc(size) malloc(size)
    #define MEMT_free(ptr) free(ptr)
    #define MEMT_print_mem_usage() {}
#endif //DEBUG

#endif //__MEMTRACK_C__
