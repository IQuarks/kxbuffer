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
 * @def REFCOUNT
 * @brief Defines a reference count member within another structure.
 * 
 * @param name The name of the reference count member.
 */
#define REFCOUNT(name) refcount_t name

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
 * @param refcount Pointer to the reference count structure to initialize.
 * @param n The initial reference count value.
 */
void INIT_REFCOUNT(REFCOUNT(*refcount), u32 n);

/**
 * @brief Acquires a reference by incrementing the count.
 * 
 * @param refcount Pointer to the reference count structure.
 * @return true if the reference was successfully acquired, false otherwise.
 */
bool refcount_acquire(REFCOUNT(*refcount));

/**
 * @brief Releases a reference by decrementing the count.
 * 
 * @param refcount Pointer to the reference count structure.
 * @return true if the last reference count was released, false otherwise.
 */
bool refcount_release(REFCOUNT(*refcount));