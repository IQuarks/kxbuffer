#pragma once
#include <entry/bool.h>

/**
 * @struct alloc_t
 * @brief Structure defining custom memory allocation and deallocation functions.
 */
typedef struct {
    void *(*alloc)(usize);
    void (*free)(void *);
} alloc_t;

/**
 * @def ALLOC_INIT
 * @brief Statically initializes an alloc_t instance with specified allocation and deallocation functions.
 * 
 * @param name Name of the alloc_t instance to be created.
 * @param _alloc Pointer to the memory allocation function.
 * @param _free Pointer to the memory deallocation function.
 */
#define ALLOC_INIT(name, _alloc, _free) alloc_t name = { _alloc, _free }

/**
 * @brief Validates whether the given alloc_t instance has valid allocation and deallocation functions.
 * 
 * @param alloc Pointer to the alloc_t instance to be validated.
 * @return true if both allocation and deallocation functions are non-null, false otherwise.
 */
static inline bool alloc_is_valid(const alloc_t *alloc) {
    return alloc && (alloc->alloc && alloc->free);
}