cmake_minimum_required(VERSION 3.18)

# [Project] kxbuffer (1.0.0)
project(kxbuffer VERSION 1.0.0 LANGUAGES C)

# Set C standard to C17
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# kxbuffer library
add_library(kxbuffer SHARED)

# kxbuffer include directory
target_include_directories(kxbuffer PUBLIC include)

# kxbuffer properties
set_target_properties(kxbuffer PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION 1
    POSITION_INDEPENDENT_CODE ON)

# kxbuffer compile options
target_compile_options(kxbuffer PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -flto=thin
    -ffreestanding
    -fvisibility=hidden)

# kxbuffer link options
target_link_options(kxbuffer PRIVATE -flto=thin -ffreestanding)

# directory for source files
add_subdirectory(src)