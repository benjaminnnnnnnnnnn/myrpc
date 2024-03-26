#set the minimun required version and name of the project
cmake_minimum_required(VERSION 3.0)
project(myrpc)

#set executable output path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set library output path
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#set where to find the .h files  -I
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/example)
#set where to search the library   -L
link_directories(${PROJECT_SOURCE_DIR}/lib)

#include myrpc source code
add_subdirectory(src)
#include example source code using the rpc
add_subdirectory(example)
