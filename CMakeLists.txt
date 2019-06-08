cmake_minimum_required(VERSION 3.14)

project(HPCM)

set(PROJ_LIB_OUTPUT_PATH bin)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)

#add_subdirectory(thead_safe_container)
#add_subdirectory(test)

add_executable(test main.cpp ./thead_safe_container/ThreadSafeStack.hpp)

