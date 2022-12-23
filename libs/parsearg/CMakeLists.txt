cmake_minimum_required(VERSION 3.14)

project(parsearg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(parsearg STATIC
    parsearg.cpp
)
add_library(whofetch::parsearg ALIAS parsearg)

target_include_directories(parsearg PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/whofetch>
)

target_link_libraries(parsearg
    util
)

set(PARSEARG_PUBLIC_HEADERS
    "parsearg.hpp"
)

set_target_properties(parsearg PROPERTIES
    PUBLIC_HEADER "${PARSEARG_PUBLIC_HEADERS}"
)

include(GNUInstallDirs)
install(TARGETS parsearg
    EXPORT whofetchTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/whofetch
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/whofetch
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/whofetch
)
