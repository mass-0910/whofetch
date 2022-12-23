cmake_minimum_required(VERSION 3.14)

project(util)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(util STATIC
    util.cpp
)
add_library(whofetch::util ALIAS util)

target_include_directories(util PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/whofetch>
)

set(PARSEARG_PUBLIC_HEADERS
    "util.hpp"
)

set_target_properties(util PROPERTIES
    PUBLIC_HEADER "${PARSEARG_PUBLIC_HEADERS}"
)

include(GNUInstallDirs)
install(TARGETS util
    EXPORT whofetchTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/whofetch
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/whofetch
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/whofetch
)
