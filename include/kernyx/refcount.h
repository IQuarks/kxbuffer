#pragma once
#include <entry/bool.h>

/**
 * @struct refcount_t
 * @brief Structure representing a reference count.
 */
typedef struct {
    u32 refs;
} refcount_t;

/**
 * @def REFCOUNT_INIT
 * @brief Statically initializes a reference count structure.
 * 
 * @param n The initial reference count value.
 */
#define REFCOUNT_INIT(n) { .refs = (n) }

/**
 * @brief Initializes a reference count structure.
 * 
 * @param refcount Pointer to the reference count structure.
 * @param n The initial reference count value.
 */
void INIT_REFCOUNT(refcount_t *refcount, u32 n);

/**
 * @brief Acquires a reference by incrementing the count.
 * 
 * @param refcount Pointer to the reference count structure.
 * @return true if the reference was successfully acquired, false otherwise.
 */
bool refcount_acquire(refcount_t *refcount);

/**
 * @brief Releases a reference by decrementing the count.
 * 
 * @param refcount Pointer to the reference count structure.
 * @return true if the last reference count was released, false otherwise.
 */
bool refcount_release(refcount_t *refcount);