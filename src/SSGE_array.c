#include <stdlib.h>
#include <stdio.h>

#include "SSGE/SSGE_array.h"

#define _INITIAL_SIZE 256
#define _IDX_PILE_INITIAL_SIZE 64
#define _GROWTH_FACTOR 2

/**
 * Creates a new array
 * \return The pointer to the new array
 */
SSGE_Array *SSGE_Array_Create() {
    SSGE_Array *array = (SSGE_Array *)malloc(sizeof(SSGE_Array));
    array->array = (void **)calloc(_INITIAL_SIZE, sizeof(void *));
    array->size = _INITIAL_SIZE;
    array->count = 0;
    array->indexes = (uint32_t *)malloc(sizeof(uint32_t) * _IDX_PILE_INITIAL_SIZE);
    array->idxSize = _IDX_PILE_INITIAL_SIZE;
    array->idxCount = 0;
}

/**
 * Adds an element to an array
 * \param array The array to add the element to
 * \return The index of the element
 */
uint32_t SSGE_Array_Add(SSGE_Array *array, void *element) {
    if (array->size <= array->count) { // if the array is full, extend
        void **newArray = (void **)realloc(array->array, sizeof(void *) * array->size * _GROWTH_FACTOR);
        if (newArray == NULL) {
            fprintf(stderr, "[SSGE][CORE] Failed to realloc array");
            exit(1);
        }
        // Set new memory to NULL
        for (uint32_t i = array->size; i < array->size * _GROWTH_FACTOR; i++) {
            array->array[i] = NULL;
        }
        array->array = newArray;
        array->size *= _GROWTH_FACTOR;
    }

    if (array->idxCount > 0) {
        array->array[array->indexes[--array->idxCount]] = element;
    } else {
        array->array[array->count] = element;
    }
    return array->count++;
}

/**
 * Gets an element from an array
 * \param array The array to get the element from
 * \param idx The index of the element to get
 * \return The pointer to the element
 */
void *SSGE_Array_Get(SSGE_Array *array, uint32_t idx) {
    if (idx >= array->size) {
        fprintf(stderr, "[SSGE][CORE] Invalid index (idx > size)");
        exit(1);
    }
    void *ret = array->array[idx];
    if (ret == NULL) {
        fprintf(stderr, "[SSGE][CORE] Invalid index (NULL)");
        exit(1);
    }
    return ret;
}

/**
 * Removes an element from an array
 * \param array The array to remove the element from
 * \param idx The index of the element to remove
 * \param destroyData The function to destroy the element to remove from the array
 */
void SSGE_Array_Remove(SSGE_Array *array, uint32_t idx, void (*destroyData)(void *)) {
    if (idx >= array->count || array->array[idx] == NULL) {
        fprintf(stderr, "[SSGE][CORE] Index out of bound");
        exit(1);
    }

    if (destroyData != NULL) destroyData(array->array[idx]);
    array->array[idx] = NULL;
    --array->count;

    if (array->idxSize <= array->idxCount) {
        uint32_t *newIndexes = (uint32_t *)realloc(array->indexes, sizeof(uint32_t) * array->idxSize * _GROWTH_FACTOR);
        if (newIndexes == NULL) {
            fprintf(stderr, "[SSGE][CORE] Failed to realloc array indexes");
            exit(1);
        }
        array->indexes = newIndexes;
        array->idxSize *= _GROWTH_FACTOR;
    }
    array->indexes[array->idxCount++] = idx;
}

/**
 * Pops an element from an array
 * \param array The array to pop the element from
 * \param idx The index of the element to pop
 * \return The pointer to the popped element
 */
void *SSGE_Array_Pop(SSGE_Array *array, uint32_t idx) {
    if (idx >= array->count || array->array[idx] == NULL) {
        fprintf(stderr, "[SSGE][CORE] Index out of bound");
        exit(1);
    }

    void *element = array->array[idx];
    array->array[idx] = NULL;
    --array->count;

    if (array->idxSize <= array->idxCount) {
        uint32_t *newIndexes = (uint32_t *)realloc(array->indexes, sizeof(uint32_t) * array->idxSize * _GROWTH_FACTOR);
        if (newIndexes == NULL) {
            fprintf(stderr, "[SSGE][CORE] Failed to realloc array indexes");
            exit(1);
        }
        array->indexes = newIndexes;
        array->idxSize *= _GROWTH_FACTOR;
    }
    array->indexes[array->idxCount++] = idx;
    return element;
}