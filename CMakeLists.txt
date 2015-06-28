cmake_minimum_required(VERSION 3.2)
project(project1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

set(SOURCE_FILES UBCalcDriver.cpp UBCalculator.cpp error_handling.cpp Lexer.cpp)
add_executable(project1 ${SOURCE_FILES})