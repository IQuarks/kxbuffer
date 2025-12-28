#include <kernyx/kx.h>
#include <kernyx/pool.h>
#include <entry/macros.h>
#include <kernyx/refcount.h>

/**
 * @brief Bitmap type for tracking slot usage.
 * @note unsigned long is used for better performance on most architectures.
 */
typedef unsigned long bitmap_t;

/**
 * @struct block_t
 * @brief Represents a block of slots in the memory pool, containing 8 * sizeof(bitmap_t) slots.
 */
typedef struct {
    u8 *slots;
    bitmap_t bitmap;
} block_t;

/**
 * @brief Creates a new block for the memory pool.
 * 
 * @param alloc Pointer to the allocator to use.
 * @param slot_size Size of each slot in the block, must be greater than 0.
 * @return Pointer to the created block, or NULL on failure.
 */
static block_t *block_create(alloc_t *alloc, usize slot_size) {
    block_t *block = alloc->alloc(sizeof(block_t));
    if (block) {
        block->slots = alloc->alloc((8 * sizeof(bitmap_t)) * slot_size);
        if (block->slots)
            block->bitmap = 0;
        else {
            alloc->free(block);
            block = NULL;
        }
    }

    return block;
}

/**
 * @brief Destroys a block and frees its resources.
 * 
 * @param alloc Pointer to the allocator to use.
 * @param block Pointer to the block to destroy.
 */
static void block_destroy(alloc_t *alloc, block_t *block) {
    block->bitmap = 0;
    alloc->free(block->slots);
    alloc->free(block);
}

/**
 * @brief Allocates a slot in the block.
 * 
 * @param block Pointer to the block.
 * @return Index of the allocated slot.
 * @note The caller must ensure that there is at least one free slot before calling this function.
 */
static u8 block_alloc_slot(block_t *block) {
    u8 idx = __builtin_ffsl(~block->bitmap) - 1;
    block->bitmap |= (1UL << idx);
    return idx;
}

/**
 * @brief Frees a slot in the block.
 * 
 * @param block Pointer to the block.
 * @param idx Index of the slot to free, must be less than 8 * sizeof(bitmap_t).
 * @return true if the slot was successfully freed, false if it was already free.
 */
static inline bool block_free_slot(block_t *block, u8 idx) {
    return (block->bitmap & (1UL << idx)) ? ((block->bitmap &= ~(1UL << idx)), true) : false;
}

/**
 * @brief Gets the index of a slot in the block.
 * 
 * @param block Pointer to the block.
 * @param slot Pointer to the slot.
 * @param slot_size Size of each slot in the block, must be greater than 0.
 * @return Index of the slot, or 0xFF if the slot is not part of the block.
 */
static u8 block_get_slot_idx(block_t *block, u8 *slot, usize slot_size) {
    u8 idx = 0xFF;
    if ((slot >= block->slots) && (slot < (block->slots + ((8 * sizeof(bitmap_t)) * slot_size)))) {
        usize offset = slot - block->slots;
        if (!(offset % slot_size))
            idx = offset / slot_size;
    }

    return idx;
}

/**
 * @brief Gets a pointer to a slot in the block.
 * 
 * @param block Pointer to the block.
 * @param idx Index of the slot, must be less than 8 * sizeof(bitmap_t).
 * @param slot_size Size of each slot in the block, must be greater than 0.
 * @return Pointer to the slot.
 */
static inline u8 *block_get_slot_ptr(block_t *block, u8 idx, usize slot_size) {
    return (block->slots + (idx * slot_size));
}

struct pool {
    alloc_t *alloc;
    refcount_t refcount;

    u8 block_count;
    usize slot_size;
    block_t **blocks;
};

__visible pool_t *pool_create(alloc_t *alloc, usize slot_size, u8 block_count) {
    pool_t *pool = (slot_size && alloc_is_valid(alloc)) ? alloc->alloc(sizeof(pool_t)) : NULL;
    if (pool) {
        pool->alloc = alloc;
        pool->blocks = NULL;
        pool->block_count = 0;
        pool->slot_size = slot_size;

        if (block_count && !pool_grow(pool, block_count)) {
            alloc->free(pool);
            return NULL;
        }

        INIT_REFCOUNT(&pool->refcount, 1U);
    }

    return pool;
}

__visible void pool_destroy(pool_t *pool) {
    if (refcount_release(&pool->refcount)) {
        alloc_t *alloc = pool->alloc;

        if (pool->blocks) {
            for (u8 i = 0; i < pool->block_count; i++)
                block_destroy(alloc, pool->blocks[i]);

            alloc->free(pool->blocks);
        }

        pool->alloc = NULL;
        pool->slot_size = 0;
        pool->block_count = 0;
        alloc->free(pool);
    }
}

__visible pool_t *pool_get_shared(pool_t *pool) {
    return refcount_acquire(&pool->refcount) ? pool : NULL;
}

__visible bool pool_grow(pool_t *pool, u8 block_count) {
    bool result = false;
    if (block_count) {
        u16 new_block_count = pool->block_count + block_count;
        if (new_block_count <= 0xFF) {
            block_t **new_blocks = pool->alloc->alloc(new_block_count * sizeof(block_t *));
            if (new_blocks) {
                for (u8 i = pool->block_count; i < new_block_count; i++) {
                    new_blocks[i] = block_create(pool->alloc, pool->slot_size);
                    if (!new_blocks[i]) {
                        for (u8 j = pool->block_count; j < i; j++)
                            block_destroy(pool->alloc, new_blocks[j]);

                        pool->alloc->free(new_blocks);
                        return false;
                    }
                }

                if (pool->blocks) {
                    for (u8 i = 0; i < pool->block_count; i++)
                        new_blocks[i] = pool->blocks[i];

                    pool->alloc->free(pool->blocks);
                }

                pool->blocks = new_blocks;
                pool->block_count = (u8) new_block_count;
                result = true;
            }
        }
    }

    return result;
}

__visible void *pool_alloc(pool_t *pool) {
    for (u8 i = 0; i < pool->block_count; i++) {
        if (pool->blocks[i]->bitmap != ~((bitmap_t) 0)) {
            u8 idx = block_alloc_slot(pool->blocks[i]);
            u8 *slot = block_get_slot_ptr(pool->blocks[i], idx, pool->slot_size);

            for (u8 j = 0; j < pool->slot_size; j++)
                slot[j] = 0;

            return slot;
        }
    }

    return NULL;
}

__visible void *pool_alloc_grow(pool_t *pool) {
    void *slot = pool_alloc(pool);
    if (!slot && pool_grow(pool, 1))
        slot = pool_alloc(pool);

    return slot;
}

__visible void pool_free(pool_t *pool, void *slot) {
    for (u8 i = 0; i < pool->block_count; i++) {
        if (pool->blocks[i]->bitmap) {
            u8 idx = block_get_slot_idx(pool->blocks[i], slot, pool->slot_size);
            if (idx != 0xFF) {
                block_free_slot(pool->blocks[i], idx);
                return;
            }
        }
    }
}