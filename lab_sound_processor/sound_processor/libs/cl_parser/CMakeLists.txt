cmake_minimum_required(VERSION 3.22)

get_filename_component(LIBRARY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "LIBRARY_NAME: ${LIBRARY_NAME}")

project(${LIBRARY_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp")
message(STATUS ${SOURCE_DIRS})

find_package(Boost REQUIRED )
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    include(FetchContent)
    FetchContent_Declare(
            Boost
            GIT_REPOSITORY https://github.com/boostorg/boost.git
            GIT_TAG boost-1.80.0
    )
    FetchContent_MakeAvailable(Boost)
endif ()

message(STATUS "project name: ${PROJECT_NAME}")
add_library(${LIBRARY_NAME} SHARED ${SOURCE_DIRS})
target_compile_features(${LIBRARY_NAME} INTERFACE cxx_std_20)

file(GLOB PRIVATE_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/*_*")
target_include_directories(${LIBRARY_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        PRIVATE
        ${PRIVATE_INCLUDES}
)

file(GLOB SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
set_target_properties(${LIBRARY_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")
target_link_libraries(${LIBRARY_NAME} ${Boost_LIBRARIES})
