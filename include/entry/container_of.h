#pragma once
#include "macros.h"

/**
 * @def container_of
 * @brief Retrieve the pointer to the containing structure from a member pointer.
 * This macro calculates the address of the containing structure given a pointer
 * to one of its members. It uses pointer arithmetic to subtract the offset of the
 * member from the member pointer, effectively yielding the address of the parent
 * structure.
 * 
 * @param ptr      Pointer to the member within the structure
 * @param type     The type of the containing structure
 * @param member   The name of the member field within the structure
 * @return A pointer to the containing structure of type `type *`
 */
#define container_of(ptr, type, member) (type *)((char *)(ptr) - offsetof(type, member))

/**
 * @def container_of_const
 * @brief Const version of container_of that preserves const qualification.
 * This macro is similar to `container_of`, but it is used when the member
 * pointer is const. It ensures that the returned pointer to the containing
 * structure is also const, preserving the const-correctness of the code.
 * 
 * @param ptr      Const pointer to the member within the structure
 * @param type     The type of the containing structure
 * @param member   The name of the member field within the structure
 * @return A const pointer to the containing structure of type `const type *`
 */
#define container_of_const(ptr, type, member) (const type *)((const char *)(ptr) - offsetof(type, member))