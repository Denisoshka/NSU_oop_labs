cmake_minimum_required(VERSION 3.8)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "project name: ${PROJECT_NAME}")

project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)
set(TEST_NAME "test_${PROJECT_NAME}")

file(GLOB CONVERTER_INTERFACE_SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
message(STATUS ${CONVERTER_INTERFACE_SOURCE_DIRS})

message(STATUS "project name: ${PROJECT_NAME}")
add_library(${PROJECT_NAME} SHARED ${CONVERTER_INTERFACE_SOURCE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC headers)

message(STATUS "Found Boost")
if (NOT Boost_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            Boost
            GIT_REPOSITORY https://github.com/boostorg/boost.git
            GIT_TAG boost-1.80.0
    )
    FetchContent_MakeAvailable(Boost)
endif()

#[[
add_subdirectory(converters)
include_directories(converters)
]]

message(STATUS "Link libraries")
target_link_libraries(${PROJECT_NAME}
        PUBLIC Boost::program_options
        PUBLIC Boost::property_tree
)


message(STATUS "Found libraries")
find_package(GTest)
if (NOT GTest_FOUND)
    include(FetchContent)
    message(STATUS "Download GTest")
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.11.0
    )
    FetchContent_MakeAvailable(googletest)
endif ()

add_executable(${TEST_NAME} test/${TEST_NAME}.cpp)
target_link_libraries(${TEST_NAME}
        gtest
        gtest_main
        ${project_name}
)

add_test(NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
