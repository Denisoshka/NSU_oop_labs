cmake_minimum_required(VERSION 3.22)
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${DIR_NAME} CXX)
set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
message(STATUS ${SOURCE_DIRS})
set(SOURCE_LIB ${SOURCE_DIRS})
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_executable(${PROJECT_NAME} ${SOURCE_LIB})
target_include_directories(${PROJECT_NAME} PUBLIC include)
