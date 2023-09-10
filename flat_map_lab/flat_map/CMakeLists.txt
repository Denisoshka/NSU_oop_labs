cmake_minimum_required(VERSION 3.8)
project(flat_map CXX)

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_LIB src/flat_map.cpp)
#set(CMAKE_CXX_COMPILER "clang++")
add_compile_options(-Wall -Wextra -pedantic -Werror)
add_library(${PROJECT_NAME} SHARED ${SOURCE_LIB})
target_include_directories(${PROJECT_NAME} PUBLIC headers)

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

add_executable(test_flat_map test/test_flat_map.cpp)

target_link_libraries(test_flat_map
        gtest
        gtest_main
        flat_map)

add_test(NAME test_flat_map
        COMMAND test_flat_map
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
