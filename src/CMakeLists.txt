add_executable(whofetch)

target_sources(whofetch PRIVATE
    main.cpp
    parsearg.cpp
    resource.cpp
    util.cpp
    whofetch.cpp
)

target_include_directories(whofetch PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR} # for cmake_config.h
    ${jp2a_INCLUDE_DIR}
)

target_link_libraries(whofetch
    jp2a
)

set_target_properties(whofetch
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

install(TARGETS whofetch
    RUNTIME DESTINATION bin
)

if(WIN32)
    install(TARGETS whofetch
        RUNTIME_DEPENDENCIES
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
        POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
    )
endif(WIN32)
