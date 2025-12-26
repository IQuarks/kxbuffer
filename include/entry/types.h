#pragma once

typedef char i8;
typedef unsigned char u8;

typedef short i16;
typedef unsigned short u16;

typedef int i32;
typedef unsigned int u32;

/**
 * @brief 64-bit integer type definitions
 * This section defines type aliases for signed and unsigned 64-bit integers.
 * The definitions take into account the target architecture (32-bit or 64-bit)
 * to ensure correct type sizes.
 * 
 * On 64-bit architectures (_LP64 defined), 'long' is used for 64-bit types.
 * On 32-bit architectures, 'long long' is used to ensure 64-bit size.
 */
#ifdef _LP64
typedef long i64;
typedef unsigned long u64;
#else
typedef long long i64;
typedef unsigned long long u64;
#endif

/**
 * @brief 128-bit integer type definitions
 * This section defines type aliases for signed and unsigned 128-bit integers,
 * if supported by the compiler. These types are useful for applications that
 * require very large integer values, such as cryptography or high-precision
 * computations.
 * 
 * @note Not all compilers support 128-bit integers. The presence of these types
 *       is checked using the __SIZEOF_INT128__ macro.
 */
#ifdef __SIZEOF_INT128__
typedef __int128 i128;
typedef unsigned __int128 u128;
#endif

typedef long ssize;
typedef unsigned long usize;

typedef float f32;
typedef double f64;