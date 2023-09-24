#[[
cmake_minimum_required(VERSION 3.8)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
#set(CMAKE_CXX_COMPILER /usr/bin/g++)
#message(STATUS ${PROJECT_NAME})
project(${PROJECT_NAME} CXX)
message(STATUS ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 20)
set(SOURCE_LIB src/flat_map.cpp)
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
        flat_map
        gtest
        gtest_main
)

add_test(NAME test_flat_map
        COMMAND test_flat_map
        WORKING_DIRECTORY test
)

]]


cmake_minimum_required(VERSION 3.8)
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${DIR_NAME} CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER /bin/clang++)

file(GLOB SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/**/flat_map.cpp")
message(STATUS ${SOURCE_DIRS})
set(SOURCE_LIB ${SOURCE_DIRS})
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_library(${PROJECT_NAME} SHARED ${SOURCE_DIRS})
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

set(TEST_NAME "test_${DIR_NAME}")

add_executable(${TEST_NAME} test/${TEST_NAME}.cpp)
target_link_libraries(${TEST_NAME} gtest gtest_main ${PROJECT_NAME})
add_test(NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)