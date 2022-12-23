cmake_minimum_required(VERSION 3.14)

project(util)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(util STATIC
    util.cpp
)
target_include_directories(util PUBLIC
    ${PROJECT_SOURCE_DIR}
)
