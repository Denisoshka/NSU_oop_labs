cmake_minimum_required(VERSION 3.22)
get_filename_component(LIBRARY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "LIBRARY_NAME: ${LIBRARY_NAME}")

project(${LIBRARY_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
message(STATUS ${SOURCE_DIRS})

message(STATUS "project name: ${PROJECT_NAME}")
add_executable(${LIBRARY_NAME} ${SOURCE_DIRS})
target_compile_options(${LIBRARY_NAME} PRIVATE -pedantic -Wall -Werror)
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/game/game_obj)

target_link_libraries(${LIBRARY_NAME} PUBLIC ${Boost_LIBRARIES} game_obj)