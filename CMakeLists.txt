cmake_minimum_required(VERSION 3.3)
project(exp1_naive)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp SimSearcher.h SimSearcher.cpp)
add_executable(exp1_naive ${SOURCE_FILES})