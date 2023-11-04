cmake_minimum_required(VERSION 3.22)

get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "LIBRARY_NAME: ${DIR_NAME}")

project(${DIR_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp")
message(STATUS ${SOURCE_DIRS})

message(STATUS "project name: ${PROJECT_NAME}")
add_library(${DIR_NAME} SHARED ${SOURCE_DIRS})
target_compile_features(${DIR_NAME} INTERFACE cxx_std_20)

file(GLOB PUBLIC_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/*screen"
)

target_include_directories(${DIR_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        ${PUBLIC_INCLUDES}
)

target_link_libraries(${DIR_NAME} PUBLIC curses)
