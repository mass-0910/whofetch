cmake_minimum_required(VERSION 3.14)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

project(whofetch
    VERSION 0.1
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Test configurations
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
if(WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif(WIN32)

# We don't want to install GoogleTest libs when we run `cmake --install`
set(INSTALL_GTEST OFF CACHE BOOL "Enable installation of GoogleTest." FORCE)

FetchContent_MakeAvailable(googletest)
enable_testing()

FetchContent_Declare(
    jp2a
    GIT_REPOSITORY https://github.com/mass-0910/jp2a
    GIT_TAG origin/master
)
FetchContent_MakeAvailable(jp2a)

configure_file(${PROJECT_SOURCE_DIR}/include/cmake_config.h.in ${CMAKE_BINARY_DIR}/cmake_config.h)

add_subdirectory(libs/parsearg)
add_subdirectory(libs/resource)
add_subdirectory(src)
add_subdirectory(test)
