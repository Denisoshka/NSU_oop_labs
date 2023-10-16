cmake_minimum_required(VERSION 3.22)
get_filename_component(LIBRARY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "LIBRARY_NAME: ${LIBRARY_NAME}")

project(${LIBRARY_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
message(STATUS ${SOURCE_DIRS})

find_package(Boost REQUIRED COMPONENTS program_options)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else ()
    include(FetchContent)
    FetchContent_Declare(
            Boost
            GIT_REPOSITORY https://github.com/boostorg/boost.git
            GIT_TAG boost-1.80.0
    )
    FetchContent_MakeAvailable(Boost)
endif ()

message(STATUS "project name: ${PROJECT_NAME}")
add_executable(${LIBRARY_NAME} ${SOURCE_DIRS})
target_compile_options(${LIBRARY_NAME} PRIVATE -pedantic -Wall -Werror)
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
include_directories(
#        ${CMAKE_CURRENT_SOURCE_DIR}/libs/converters
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/wav
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/cl_parser)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/converters)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/wav)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/cl_parser)

target_link_libraries(${LIBRARY_NAME} PUBLIC ${Boost_LIBRARIES} converters wav cl_parser Boost::program_options)
