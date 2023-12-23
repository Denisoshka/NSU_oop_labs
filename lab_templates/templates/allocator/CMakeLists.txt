cmake_minimum_required(VERSION 3.22)
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS ${DIR_NAME})
project(${DIR_NAME} CXX)
set(CMAKE_CXX_STANDARD 20)
#file(GLOB SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
message(STATUS ${SOURCE_DIRS})
set(SOURCE_LIB ${SOURCE_DIRS})
#set(CMAKE_BUILD_TYPE Release)
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)

set(TEST_NAME "test_${DIR_NAME}")

add_executable(${TEST_NAME} test/${TEST_NAME}.cpp)
target_link_libraries(${TEST_NAME} ${PROJECT_NAME})

