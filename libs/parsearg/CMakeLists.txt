cmake_minimum_required(VERSION 3.14)

project(parsearg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(parsearg STATIC
    parsearg.cpp
)
target_include_directories(parsearg PUBLIC
    ${PROJECT_SOURCE_DIR}
)
target_link_libraries(parsearg
    util
)
