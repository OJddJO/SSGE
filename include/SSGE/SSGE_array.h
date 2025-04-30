#ifndef __SSGE_ARRAY_H__
#define __SSGE_ARRAY_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct _SSGE_Array {
    void **array; // Pointer to the array of pointers
    uint32_t size; // Size of the array
    uint32_t count; // Number of elements in the array
    uint32_t *indexes; // Pointer to the pile of unused indexes
    uint32_t idxSize; // The size of the pile of indexes
    uint32_t idxCount; // Number of unused indexes
} SSGE_Array;

SSGE_Array *SSGE_Array_Create();
uint32_t SSGE_Array_Add(SSGE_Array *array, void *element);
void *SSGE_Array_Get(SSGE_Array *array, uint32_t idx);
void SSGE_Array_Remove(SSGE_Array *array, uint32_t idx, void (*destroyData)(void *));
void *SSGE_Array_Pop(SSGE_Array *array, uint32_t idx);
void *SSGE_Array_Find(SSGE_Array * array, void *element, bool (*condition)(void *));

#endif // __SSGE_ARRAY_H__