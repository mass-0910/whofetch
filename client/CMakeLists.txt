add_executable(whofetch
    src/main.cpp
    src/whofetch.cpp
)

target_include_directories(whofetch PUBLIC include jp2a/include)
target_link_libraries(whofetch lib)
target_link_libraries(whofetch jp2a)

include(FetchContent)
FetchContent_Declare(
    jp2a
    GIT_REPOSITORY https://github.com/mass-0910/jp2a
    GIT_TAG origin/master
)

FetchContent_MakeAvailable(jp2a)