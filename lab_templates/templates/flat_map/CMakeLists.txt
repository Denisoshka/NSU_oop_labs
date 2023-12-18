cmake_minimum_required(VERSION 3.22)
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${DIR_NAME} CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/**/flat_map.cpp")
message(STATUS ${SOURCE_DIRS})
set(SOURCE_LIB ${SOURCE_DIRS})
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/allocator)
add_library(${PROJECT_NAME} INTERFACE ${SOURCE_LIB})
target_include_directories(${PROJECT_NAME} INTERFACE include)


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

set(TEST_NAME "test_${DIR_NAME}")
add_executable(${TEST_NAME} test/${TEST_NAME}.cpp)
target_link_libraries(${TEST_NAME} ${PROJECT_NAME} gtest gtest_main allocator)

