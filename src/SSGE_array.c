#include <stdlib.h>

#include "SSGE/SSGE_array.h"
#include "SSGE/SSGE_error.h"

#define _INITIAL_SIZE 256
#define _IDX_PILE_INITIAL_SIZE 64
#define _GROWTH_FACTOR 2

/**
 * Creates a new array
 * \param array The array to initialize
 */
SSGEAPI void SSGE_Array_Create(SSGE_Array *array) {
    array->array = (void **)calloc(_INITIAL_SIZE, sizeof(void *));
    if (array->array == NULL) 
        SSGE_Error("Failed to allocate memory for array")
    array->size = _INITIAL_SIZE;
    array->count = 0;
    array->indexes = (uint32_t *)malloc(sizeof(uint32_t) * _IDX_PILE_INITIAL_SIZE);
    if (array->indexes == NULL) 
        SSGE_Error("Failed to allocate memory for array indexes pile")
    array->idxSize = _IDX_PILE_INITIAL_SIZE;
    array->idxCount = 0;
}

/**
 * Adds an element to an array
 * \param array The array to add the element to
 * \return The index of the element
 */
SSGEAPI uint32_t SSGE_Array_Add(SSGE_Array *array, void *element) {
    if (array->size <= array->count) { // if the array is full, extend
        void **newArray = (void **)realloc(array->array, sizeof(void *) * array->size * _GROWTH_FACTOR);
        if (newArray == NULL) 
            SSGE_Error("Failed to realloc array")
        array->array = newArray;

        // Set new memory to NULL
        for (uint32_t i = array->size; i < array->size * _GROWTH_FACTOR; i++)
            array->array[i] = NULL;

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
SSGEAPI void *SSGE_Array_Get(SSGE_Array *array, uint32_t idx) {
    if (idx >= array->size)
        return NULL;

    return array->array[idx];
}

/**
 * Removes an element from an array
 * \param array The array to remove the element from
 * \param idx The index of the element to remove
 * \param destroyData The function to destroy the element to remove from the array
 */
SSGEAPI void SSGE_Array_Remove(SSGE_Array *array, uint32_t idx, void (*destroyData)(void *)) {
    if (idx >= array->size) {
        return;
    }
    if (array->array[idx] == NULL) {
        return;
    }

    if (destroyData != NULL) destroyData(array->array[idx]);
    array->array[idx] = NULL;
    --array->count;

    if (array->idxSize <= array->idxCount) {
        uint32_t *newIndexes = (uint32_t *)realloc(array->indexes, sizeof(uint32_t) * array->idxSize * _GROWTH_FACTOR);
        if (newIndexes == NULL) 
            SSGE_Error("Failed to realloc array indexes pile");
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
SSGEAPI void *SSGE_Array_Pop(SSGE_Array *array, uint32_t idx) {
    if (idx >= array->size) 
        return NULL;

    void *element = array->array[idx];
    array->array[idx] = NULL;
    --array->count;

    if (array->idxSize <= array->idxCount) {
        uint32_t *newIndexes = (uint32_t *)realloc(array->indexes, sizeof(uint32_t) * array->idxSize * _GROWTH_FACTOR);
        if (newIndexes == NULL) 
            SSGE_Error("Failed to realloc array indexes pile")
        array->indexes = newIndexes;
        array->idxSize *= _GROWTH_FACTOR;
    }
    array->indexes[array->idxCount++] = idx;
    return element;
}

/**
 * Finds an element from an array with a condition
 * \param array The array to find the element from
 * \param condition The condition to find the element. Must be `bool condition(void *element, void *argument)`
 * \param argument The argument to pass to the condition function
 * \return The pointer to the first element that matches the condition, or NULL if not found
 */
SSGEAPI void *SSGE_Array_Find(SSGE_Array *array, bool (*condition)(void *, void *), void *argument) {
    uint32_t i = 0, count = 0;
    while (count < array->count || count >= array->size) {
        void *element = array->array[i++];

        if (element == NULL) continue;
        if (condition(element, argument)) return element;

        count++;
    }
    return NULL;
}

/**
 * Finds an element from array with a condition and pops it
 * \param array The array to find and pop the element from
 * \param condition The condition to find the element. Must be `bool condition(void *element, void *argument)`
 * \param argument The argument to pass to the condition function
 * \return The pointer to the first element that matches the condition, or NULL if not found
 */
SSGEAPI void *SSGE_Array_FindPop(SSGE_Array *array, bool (*condition)(void *, void *), void *argument) {
    uint32_t i = 0, count = 0;
    while (count < array->count || count >= array->size) {
        void *element = array->array[i++];

        if (element == NULL) continue;
        if (condition(element, argument)) return SSGE_Array_Pop(array, i);

        ++count;
    }
    return NULL;
}

/**
 * Destroys an array
 * \param array The array to destroy
 * \param destroyData The function to destroy the elements of the array. Can be `NULL`
 */
SSGEAPI void SSGE_Array_Destroy(SSGE_Array *array, void (*destroyData)(void *)) {
    uint32_t i = 0, count = 0;
    while (count < array->count || count >= array->size) {
        void *element = array->array[i++];

        if (element == NULL) continue;
        if (destroyData) destroyData(element);

        ++count;
    }
    free(array->array);
    free(array->indexes);
}