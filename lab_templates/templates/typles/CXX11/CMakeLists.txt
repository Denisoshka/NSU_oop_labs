cmake_minimum_required(VERSION 3.22)
get_filename_component(LIBRARY_NAME .. NAME)
message(STATUS "LIBRARY_NAME: ${LIBRARY_NAME}")

project(${LIBRARY_NAME} CXX)

set(CMAKE_CXX_STANDARD 11)
add_library(${PROJECT_NAME} SHARED just.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)


