cmake_minimum_required(VERSION 3.19)
project(28_threads)

set(CMAKE_CXX_STANDARD 20)

add_executable(28_threads fast_start.cpp delete.cpp threads_test.cpp)
add_executable(test delete.cpp)
add_executable(threads_test threads_test.cpp)
add_executable(task_1 tasks/task_1.cpp)