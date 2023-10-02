cmake_minimum_required(VERSION 3.8)

get_filename_component(LIBRARY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "LIBRARY_NAME: ${LIBRARY_NAME}")

project(${LIBRARY_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)
#set(TEST_NAME "test_${LIBRARY_NAME}")

file(GLOB SOURCE_DIRS  "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp")
message(STATUS ${SOURCE_DIRS})

message(STATUS "project name: ${PROJECT_NAME}")
add_library(${LIBRARY_NAME} SHARED ${SOURCE_DIRS})
target_include_directories(${LIBRARY_NAME} PUBLIC include)


#message(STATUS "Found libraries")
#find_package(GTest)
#if (NOT GTest_FOUND)
#    include(FetchContent)
#    message(STATUS "Download GTest")
#    FetchContent_Declare(
#            googletest
#            GIT_REPOSITORY https://github.com/google/googletest.git
#            GIT_TAG release-1.11.0
#    )
#    FetchContent_MakeAvailable(googletest)
#endif ()
#
#add_executable(${TEST_NAME} test/${TEST_NAME}.cpp)
#
#target_link_libraries(${TEST_NAME}
#        gtest
#        gtest_main
#        ${LIBRARY_NAME}
#)
#
#add_test(NAME ${TEST_NAME}
#        COMMAND ${TEST_NAME}
#        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
