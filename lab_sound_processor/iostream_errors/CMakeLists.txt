cmake_minimum_required(VERSION 3.8)

get_filename_component(LIBRARY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "LIBRARY_NAME: ${LIBRARY_NAME}")

project(${LIBRARY_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
message(STATUS ${SOURCE_DIRS})

message(STATUS "project name: ${PROJECT_NAME}")
add_library(${LIBRARY_NAME} SHARED ${SOURCE_DIRS})
target_include_directories(${LIBRARY_NAME} PUBLIC include)
