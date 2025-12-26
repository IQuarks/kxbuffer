#pragma once

/**
 * @def __visible
 * @brief Marks a symbol as visible outside the shared library.
 */
#define __visible __attribute__((visibility("default")))

/**
 * @def __hidden
 * @brief Marks a symbol as hidden within the shared library.
 */
#define __hidden __attribute__((visibility("hidden")))