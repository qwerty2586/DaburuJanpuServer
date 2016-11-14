cmake_minimum_required(VERSION 3.6)
project(DaburuJanpuServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp client.cpp client.h server.cpp server.h string_functions.cpp string_functions.h client_list.cpp client_list.h command_queue.cpp command_queue.h command.cpp command.h lobby_utils.cpp lobby_utils.h)
add_executable(DaburuJanpuServer ${SOURCE_FILES})