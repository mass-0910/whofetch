add_executable(lib_test
    ../src/parsearg.cpp
    ../src/resource.cpp
    ../src/util.cpp
    lib_test.cpp
)

target_include_directories(lib_test PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR} # for cmake_config.h
)
target_link_libraries(lib_test GTest::gtest_main)

set_target_properties(lib_test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

include(GoogleTest)
gtest_discover_tests(lib_test)
