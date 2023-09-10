cmake_minimum_required(VERSION 3.8)

get_filename_component(project_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(${project_name})

project(${project_name} CXX)

set(CMAKE_CXX_STANDARD 20)
set(TEST_NAME "test_${project_name}")

file(GLOB CONVERTER_INTERFACE_SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} OBJECT ${CONVERTER_INTERFACE_SOURCE_DIRS})

message(STATUS "Found libraries")
find_package(GTest)
if (NOT GTest_FOUND)
    include(FetchContent)
    message(STATUS "download GTest")
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

message(STATUS "Link libraries")
message(STATUS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}
        PUBLIC converters
        PUBLIC Boost::program_options
        PUBLIC Boost::property_tree
)

add_executable(${TEST_NAME} test/${TEST_NAME}.cpp)
target_link_libraries(${TEST_NAME}
        gtest
        gtest_main
        ${project_name}
)

add_test(NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
