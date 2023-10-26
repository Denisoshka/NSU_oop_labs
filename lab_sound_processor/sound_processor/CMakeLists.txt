cmake_minimum_required(VERSION 3.22)
get_filename_component(PROGRAM_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "PROGRAM_NAME: ${PROGRAM_NAME}")

project(${PROGRAM_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
message(STATUS ${SOURCE_DIRS})

find_package(Boost REQUIRED COMPONENTS program_options)
if (NOT Boost_FOUND)
  include(FetchContent)
  FetchContent_Declare(
          Boost
          GIT_REPOSITORY https://github.com/boostorg/boost.git
          GIT_TAG boost-1.80.0
  )
  FetchContent_MakeAvailable(Boost)
else ()
  include_directories(${Boost_INCLUDE_DIRS})
endif ()

message(STATUS "project name: ${PROJECT_NAME}")
add_executable(${PROGRAM_NAME} ${SOURCE_DIRS})
target_compile_options(${PROGRAM_NAME} PRIVATE -pedantic -Wall -Werror)
target_include_directories(${PROGRAM_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/wav)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/cl_parser)


set(LIB_N 0)
if (${LIB_N} EQUAL 0)
  message(STATUS "use TESTLIB_1 lib")
  add_library(converters SHARED IMPORTED)
  target_include_directories(${PROGRAM_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs/converters/include)
  set_target_properties(converters PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/din_test/libconverters.so)
else ()
  message(STATUS "use CONVERTERS lib")
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/converters)
endif ()

target_link_libraries(${PROGRAM_NAME} PUBLIC ${Boost_LIBRARIES} converters wav cl_parser Boost::program_options)
