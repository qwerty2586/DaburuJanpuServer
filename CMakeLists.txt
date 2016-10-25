cmake_minimum_required(VERSION 3.6)
project(DaburuJanpuServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp client.cpp client.h server.cpp server.h split.cpp split.h)
add_executable(DaburuJanpuServer ${SOURCE_FILES})