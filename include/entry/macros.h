#pragma once

#ifndef NULL
#ifndef __cplusplus
#define NULL ((void *) 0)
#else
#define NULL static_cast<typeof(nullptr)>(0)
#endif
#endif

#ifndef typeof
#ifndef __cplusplus
#define typeof __typeof__
#else
#define typeof decltype
#endif
#endif

#ifndef offsetof
#define offsetof __builtin_offsetof
#endif

/**
 * @def READ_ONCE
 * @brief Read a variable in a way that prevents compiler optimizations
 *        that could lead to multiple reads or reordering.
 * This macro reads the value of a variable in a manner that
 * prevents the compiler from optimizing the access in a way
 * that could result in multiple reads or reordering of operations.
 * 
 * @param x The variable to read.
 * @return The value of the variable 'x'.
 */
#define READ_ONCE(x) (*(volatile typeof(x) *) &(x))

/**
 * @def WRITE_ONCE
 * @brief Write a value to a variable in a way that prevents compiler
 *        optimizations that could lead to multiple writes or reordering.
 * This macro writes a value to a variable in a manner that
 * prevents the compiler from optimizing the access in a way
 * that could result in multiple writes or reordering of operations.
 * 
 * @param x The variable to write to.
 * @param val The value to write to the variable 'x'.
 */
#define WRITE_ONCE(x, val) do { \
        typeof(val) __val__ = (val); \
        *(volatile typeof(x) *) &(x) = *(typeof(val) *) &__val__; \
    } while (0)