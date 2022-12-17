add_executable(whofetch
    src/main.cpp
    src/whofetch.cpp
)

add_subdirectory(jp2a)
target_include_directories(whofetch PUBLIC include jp2a/include)
target_link_libraries(whofetch lib)
target_link_libraries(whofetch jp2a)