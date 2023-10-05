cmake_minimum_required(VERSION 3.8)

get_filename_component(PROGRAM_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "LIBRARY_NAME: ${LIBRARY_NAME}")

project(${PROGRAM_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)

#message(STATUS ${SOURCE_DIRS})
find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
#target_link_libraries(${LIBRARY_NAME} ${Boost_LIBRARIES})

message(STATUS "project name: ${PROJECT_NAME}")
add_executable(${PROGRAM_NAME} main.cpp)

link_directories(${CMAKE_SOURCE_DIR}/libs)
link_directories(${CMAKE_SOURCE_DIR}/process)


target_link_libraries(${PROGRAM_NAME} cl_parser process ${Boost_LIBRARIES})
