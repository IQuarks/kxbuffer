#pragma once
#include "refcount.h"

/**
 * @struct alloc_t
 * @brief Structure representing a memory allocator with reference counting.
 */
typedef struct {
    REFCOUNT(refcount);

    void *(*alloc)(usize);
    void *(*realloc)(void *, usize);
    void (*free)(void *);
} alloc_t;

/**
 * @def ALLOC
 * @brief Defines an alloc_t variable or member within another structure.
 * 
 * @param name The name of the alloc_t variable.
 */
#define ALLOC(name) alloc_t name

/**
 * @def ALLOC_INIT
 * @brief Initializes an alloc_t variable with specified function pointers and a reference count of 1.
 * 
 * @param name The name of the alloc_t variable.
 * @param _alloc Pointer to the allocation function.
 * @param _realloc Pointer to the reallocation function.
 * @param _free Pointer to the free function.
 */
#define ALLOC_INIT(name, _alloc, _realloc, _free) ALLOC(name) = { \
        REFCOUNT_INIT(1U), \
        .alloc = (_alloc), \
        .realloc = (_realloc), \
        .free = (_free), \
    }

/**
 * @brief Initializes an alloc_t structure.
 * 
 * @param alloc Pointer to the alloc_t structure to initialize.
 * @param _alloc Pointer to the allocation function.
 * @param _realloc Pointer to the reallocation function.
 * @param _free Pointer to the free function.
 * @return true if initialization is successful, false otherwise.
 */
bool INIT_ALLOC(ALLOC(*alloc), void *(*_alloc)(usize), void *(*_realloc)(void *, usize), void (*_free)(void *));

/**
 * @def alloc_on_destroy
 * @brief Releases a reference before destroying an alloc_t structure.
 * 
 * @param alloc Pointer to the alloc_t structure.
 * @return true if the last reference was released, false otherwise.
 */
#define alloc_on_destroy(alloc) refcount_release(&(alloc)->refcount)

/**
 * @brief Gets a reference to the alloc_t structure.
 * 
 * @param alloc Pointer to the alloc_t structure.
 * @return Pointer to the alloc_t structure if successful, NULL otherwise.
 */
void *alloc_get_ref(ALLOC(*alloc));