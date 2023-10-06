cmake_minimum_required(VERSION 3.22)
get_filename_component(LIBRARY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "LIBRARY_NAME: ${LIBRARY_NAME}")

project(${LIBRARY_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
message(STATUS ${SOURCE_DIRS})

find_package(Boost REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else ()
    include(FetchContent)
    FetchContent_Declare(
            Boost
            GIT_REPOSITORY https://github.com/boostorg/boost.git
            GIT_TAG boost-1.80.0
    )
    FetchContent_MakeAvailable(Boost)
endif ()

message(STATUS "project name: ${PROJECT_NAME}")
add_compile_options(-Werror -Wall -Wextra -Wpedantic)
add_executable(${LIBRARY_NAME} ${SOURCE_DIRS})

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/converters
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/wav
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/cl_parser)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/converters)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/wav)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/cl_parser)

target_link_libraries(${LIBRARY_NAME} PUBLIC ${Boost_LIBRARIES} converters wav cl_parser)

#[[

cmake_minimum_required(VERSION 3.27)

get_filename_component(LIBRARY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "LIBRARY_NAME: ${LIBRARY_NAME}")

project(${LIBRARY_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)
#set(TEST_NAME "test_${LIBRARY_NAME}")

#file(GLOB SOURCE_DIRS  "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp")
message(STATUS ${SOURCE_DIRS})

message(STATUS "project name: ${PROJECT_NAME}")
#add_library(${LIBRARY_NAME} SHARED ${SOURCE_DIRS})
add_executable(${LIBRARY_NAME} src/process.cpp)
target_include_directories(${LIBRARY_NAME} PUBLIC include ../libs/)
target_include_directories(${LIBRARY_NAME} PRIVATE ../libs/converters ../libs/wav)
link_directories(../libs/converters ../libs/wav)
target_link_libraries(${LIBRARY_NAME} ../libs/converters ../libs/wav)
#message(STATUS "Found libraries")
#find_package(GTest)
#if (NOT GTest_FOUND)
#    include(FetchContent)
#    message(STATUS "Download GTest")
#    FetchContent_Declare(
#            googletest
#            GIT_REPOSITORY https://github.com/google/googletest.git
#            GIT_TAG release-1.11.0
#    )
#    FetchContent_MakeAvailable(googletest)
#endif ()
#
#add_executable(${TEST_NAME} test/${TEST_NAME}.cpp)
#
#target_link_libraries(${TEST_NAME}
#        gtest
#        gtest_main
#        ${LIBRARY_NAME}
#)
#
#add_test(NAME ${TEST_NAME}
#        COMMAND ${TEST_NAME}
#        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
]]
