cmake_minimum_required(VERSION 3.11)
project(cpp-test VERSION 1.0.0)

# SET(CMAKE_VERBOSE_MAKEFILE on)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3")

# test 1
add_executable(test_static test/staticClassFree.cpp)

# test 2
add_executable(test_child_thread_info test/childThreadInfo.cpp)
target_link_libraries(test_child_thread_info pthread)

# test 2
add_executable(test_child_process_info test/childProcessInfo.cpp)
target_link_libraries(test_child_thread_info pthread)