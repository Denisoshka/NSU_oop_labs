cmake_minimum_required(VERSION 3.8)

#get_filename_component(project_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(project_name converters)
message(${project_name})

project(${project_name} CXX)

set(CMAKE_CXX_STANDARD 20)
set(TEST_NAME "test_${project_name}")

file(GLOB CONVERTER_SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
#[[
string(REPLACE
        "${CMAKE_CURRENT_SOURCE_DIR}/"
        " "
        CONVERTERS
        ${CONVERTER_SOURCE_DIRS}
)
]]

#MESSAGE(${CONVERTER_SOURCE_DIRS})
#set(CONVERTERS "src/mix/mix.cpp src/mute/mute.cpp src/converter.cpp")

#message(${CONVERTERS})

#message(${PROJECT_NAME})
#message("------" ${CMAKE_CURRENT_SOURCE_DIR})
#[[
foreach (CONVERTER_SOURCE_DIR ${CONVERTER_SOURCE_DIRS})
    add_subdirectory(${CONVERTER_SOURCE_DIR})
endforeach (CONVERTER_SOURCE_DIR)
]]

#set(SOURCE_LIB CONVERTERS)

add_compile_options(-Wall -Wextra -pedantic -Werror)
add_library(${PROJECT_NAME} OBJECT ${CONVERTER_SOURCE_DIRS})
target_include_directories(${project_name} PUBLIC headers)

message(STATUS "Found libraries")
find_package(GTest)
if (NOT GTest_FOUND)
    include(FetchContent)
#    message(STATUS "download GTest")
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.11.0
    )
    FetchContent_MakeAvailable(googletest)
endif ()



if (NOT Boost_FOUND)
    include(FetchContent)
#    message(STATUS "download Boost1")
    FetchContent_Declare(
            Boost
            GIT_REPOSITORY https://github.com/boostorg/boost.git
            GIT_TAG boost-1.80.0
    )
#    message(STATUS "download Boost2")
    FetchContent_MakeAvailable(Boost)
    #    message(STATUS "download Boost3")
endif ()
message(STATUS "Link libraries")

target_link_libraries(${PROJECT_NAME}
        PUBLIC Boost::property_tree
)


add_executable(${TEST_NAME} test/${TEST_NAME}.cpp)
target_link_libraries(${TEST_NAME}
        gtest
        gtest_main
        ${project_name}
)

add_test(NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
