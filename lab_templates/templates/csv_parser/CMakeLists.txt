cmake_minimum_required(VERSION 3.22)
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS ${DIR_NAME})
project(${DIR_NAME} CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(${DIR_NAME} INTERFACE )
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tuple/tuple_cxx20)
target_include_directories(${DIR_NAME} INTERFACE include)
target_link_libraries(${DIR_NAME} INTERFACE tuple_cxx20)

set(TEST_NAME "test_${DIR_NAME}")
add_executable(${TEST_NAME} test/${TEST_NAME}.cpp)
target_link_libraries(${TEST_NAME} ${DIR_NAME})
