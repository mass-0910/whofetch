cmake_minimum_required(VERSION 3.14)

project(resource)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(resource STATIC
    resource.cpp
)
target_include_directories(resource PUBLIC
    ${PROJECT_SOURCE_DIR}
)
