#pragma once
#include "alloc.h"

/**
 * @struct pool_t
 * @brief Memory pool structure for managing fixed-size slot allocations.
 */
typedef struct pool pool_t;

/**
 * @brief Creates a new memory pool.
 * 
 * @param alloc Pointer to the allocator to use.
 * @param slot_size Size of each slot in the pool.
 * @param block_count Initial number of blocks to allocate for the pool.
 * @return Pointer to the created pool, or NULL on failure.
 */
pool_t *pool_create(alloc_t *alloc, usize slot_size, u8 block_count);

/**
 * @brief Destroys a memory pool and frees its resources.
 * 
 * @param pool Pointer to the pool to destroy.
 */
void pool_destroy(pool_t *pool);

/**
 * @brief Increases the reference count of the memory pool.
 * 
 * @param pool Pointer to the pool.
 * @return Pointer to the same pool.
 */
pool_t *pool_get_shared(pool_t *pool);

/**
 * @brief Grows the memory pool by allocating additional blocks.
 * 
 * @param pool Pointer to the pool to grow.
 * @param block_count Number of additional blocks to allocate.
 * @return true if the pool was successfully grown, false otherwise.
 */
bool pool_grow(pool_t *pool, u8 block_count);

/**
 * @brief Allocates a slot from the memory pool.
 * 
 * @param pool Pointer to the pool to allocate from.
 * @return Pointer to the allocated slot, or NULL if no slots are available.
 */
void *pool_alloc(pool_t *pool);

/**
 * @brief Allocates a slot from the memory pool, growing the pool if necessary.
 * 
 * @param pool Pointer to the pool to allocate from.
 * @return Pointer to the allocated slot, or NULL on failure.
 */
void *pool_alloc_grow(pool_t *pool);

/**
 * @brief Frees a previously allocated slot back to the memory pool.
 * 
 * @param pool Pointer to the pool to free the slot to.
 * @param slot Pointer to the slot to free.
 */
void pool_free(pool_t *pool, void *slot);