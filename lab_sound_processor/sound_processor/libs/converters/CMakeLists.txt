cmake_minimum_required(VERSION 3.22)

get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "LIBRARY_NAME: ${DIR_NAME}")

project(${DIR_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp")
message(STATUS ${SOURCE_DIRS})

find_package(Boost REQUIRED COMPONENTS regex)
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
add_library(${DIR_NAME} SHARED ${SOURCE_DIRS})
target_compile_features(${DIR_NAME} INTERFACE cxx_std_20)

file(GLOB PRIVATE_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/*_*")
target_include_directories(${DIR_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        PRIVATE
        ${PRIVATE_INCLUDES}
)

file(GLOB SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
set_target_properties(${DIR_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")
target_link_libraries(${DIR_NAME} Boost::regex)

#tests
#[[

set(TEST_NAME "test_${DIR_NAME}")
add_executable(${TEST_NAME} test/${TEST_NAME}.cpp)
target_link_libraries(${TEST_NAME} ${PROJECT_NAME}
        gtest
        gtest_main
        Boost::regex)

add_test(NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
        ]]
