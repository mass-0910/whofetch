add_library(lib STATIC util.cpp parsearg.cpp resource.cpp)

add_executable(lib_test test/lib_test.cpp)
target_link_libraries(lib_test lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(lib_test)

enable_testing()