cmake_minimum_required(VERSION 3.8)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "LIBRARY_NAME: ${LIBRARY_NAME}")

project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)
set(TEST_NAME "test_${LIBRARY_NAME}")

add_subdirectory(cl_parser)
add_subdirectory(converter_interface)
add_subdirectory(iostream_errors)

add_executable(main sound_processor/main.cpp)
message(STATUS "Link libraries")
target_link_libraries(main
        PUBLIC converter_interface
        PUBLIC cl_parser
)


