cmake_minimum_required(VERSION 3.14)

project(
  parser
  VERSION 1.0
  DESCRIPTION "Simple program for basic expression parsing"
  LANGUAGES CXX
)

add_library(
    BasicTypes 
    basic_classes.hpp 
    basic_classes.cpp
)

add_executable(${PROJECT_NAME} parser.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE BasicTypes)
