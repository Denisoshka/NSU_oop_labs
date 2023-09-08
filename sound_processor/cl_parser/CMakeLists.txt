cmake_minimum_required(VERSION 3.8)
project(cl_parser CXX)

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_LIB src/cl_parser.cpp)
#set(CMAKE_CXX_COMPILER "clang++")
add_compile_options(-Wall -Wextra -pedantic -Werror)
add_library(${PROJECT_NAME} OBJECT ${SOURCE_LIB})
target_include_directories(cl_parser PUBLIC headers)

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

if (NOT Boost_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            Boost
            GIT_REPOSITORY https://github.com/boostorg/boost.git
            GIT_TAG boost-1.80.0
    )
    FetchContent_MakeAvailable(Boost)
endif()

target_link_libraries(cl_parser
        PUBLIC Boost::program_options
        PUBLIC Boost::property_tree
)

add_executable(test_cl_parser test/test_cl_parser.cpp)

target_link_libraries(test_cl_parser
        gtest
        gtest_main
        cl_parser
)

add_test(NAME test_cl_parser
        COMMAND test_cl_parser
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
