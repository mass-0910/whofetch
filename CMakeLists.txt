cmake_minimum_required(VERSION 3.12)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

project(whofetch
    VERSION 0.1
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(include/config.h.in config.h)

include_directories(include ${PROJECT_BINARY_DIR})

add_subdirectory(lib)
add_subdirectory(client)