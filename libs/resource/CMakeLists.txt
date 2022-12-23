cmake_minimum_required(VERSION 3.14)

project(resource)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(resource STATIC
    resource.cpp
)
add_library(whofetch::resource ALIAS resource)
target_include_directories(resource PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/whofetch>
)

set_target_properties(resource PROPERTIES
    PUBLIC_HEADER "resource.hpp"
)

include(GNUInstallDirs)
install(TARGETS resource
    EXPORT whofetchTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/whofetch
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/whofetch
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/whofetch
)
