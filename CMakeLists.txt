cmake_minimum_required(VERSION 2.6)
#set(CMAKE_CXX_COMPILER clang)
add_definitions(-std=c++0x -I.)
add_executable(algo_test rbtree_algorithm_test.cpp)
