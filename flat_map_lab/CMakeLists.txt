cmake_minimum_required(VERSION 3.8)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)
#set(SOURCE_LIB src/flat_map.cpp)
#set(CMAKE_CXX_COMPILER "clang++")
add_compile_options(-Wall -Wextra -pedantic -Werror)
#include_directories(flat_map)
add_subdirectory(../lab_flat_map/flat_map)
add_executable(main ../lab_flat_map/main.cpp)


find_package(GTest)
if (NOT GTest_FOUND)
    include(FetchContent)

    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.11.0
    )
    FetchContent_MakeAvailable(googletest)
endif ()


target_link_libraries(main
        ../lab_flat_map/flat_map
        gtest
        gtest_main
)
