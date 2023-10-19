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

file(GLOB PUBLIC_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/converter"
        "${CMAKE_CURRENT_SOURCE_DIR}/factory"
        "${CMAKE_CURRENT_SOURCE_DIR}/other_tools"
)

file(GLOB PRIVATE_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/*_*")

target_include_directories(${DIR_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        ${PUBLIC_INCLUDES}
        PRIVATE
        ${PRIVATE_INCLUDES}
)

file(GLOB SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
#set_target_properties(${DIR_NAME} PROPERTIES ${SOURCE_DIRS} )
target_link_libraries(${DIR_NAME} Boost::regex)

set(OUTPUT_JSON "${CMAKE_CURRENT_SOURCE_DIR}/converters_description.json")
file(GLOB CONVERTER_JSONS "${CMAKE_CURRENT_SOURCE_DIR}/*_*/info.json")

execute_process(
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/combine_json.py ${OUTPUT_JSON} ${CONVERTER_JSONS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE SCRIPT_RESULT
)

if (SCRIPT_RESULT EQUAL 0)
  message(STATUS "converters description generated")
else ()
  message(STATUS "converters description not generated")
endif ()

file(READ ${OUTPUT_JSON} OUTPUT_JSON_INFO)
string(REPLACE  "\"" "\\\"" OUTPUT_JSON_INFO "${OUTPUT_JSON_INFO}" )
string(REPLACE  "\n" "" OUTPUT_JSON_INFO "${OUTPUT_JSON_INFO}" )
target_compile_definitions(${DIR_NAME} PRIVATE CONVERTERS_DESC="${OUTPUT_JSON_INFO}")
