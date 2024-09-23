cmake_minimum_required(VERSION 3.11)
project(cpp-test VERSION 1.0.0)

# SET(CMAKE_VERBOSE_MAKEFILE on)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3")

add_executable(test_static test/staticClassFree.cpp)