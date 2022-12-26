add_executable(lib_test
    lib_test.cpp
)

target_include_directories(lib_test PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(lib_test
    GTest::gtest_main
    parsearg
    resource
)

set_target_properties(lib_test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

include(GoogleTest)
gtest_discover_tests(lib_test)
