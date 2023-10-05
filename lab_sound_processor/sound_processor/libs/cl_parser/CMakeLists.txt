cmake_minimum_required(VERSION 3.8)

get_filename_component(LIBRARY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "LIBRARY_NAME: ${LIBRARY_NAME}")

project(${LIBRARY_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)
set(TEST_NAME "test_${LIBRARY_NAME}")

file(GLOB SOURCE_DIRS  "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp")
message(STATUS ${SOURCE_DIRS})

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

message(STATUS "project name: ${PROJECT_NAME}")
add_library(${LIBRARY_NAME} SHARED ${SOURCE_DIRS})
target_include_directories(${LIBRARY_NAME} PUBLIC include )
target_link_libraries(${LIBRARY_NAME} FOR ${Boost_LIBRARIES})