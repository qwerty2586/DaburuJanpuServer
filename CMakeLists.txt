cmake_minimum_required(VERSION 3.6)
project(DaburuJanpuServer)
# definice LINUX
if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif ()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if (LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
endif ()

set(SOURCE_FILES main.cpp client.cpp client.h server.cpp server.h string_functions.cpp string_functions.h client_list.cpp client_list.h command_queue.cpp command_queue.h command.cpp command.h lobby_utils.cpp lobby_utils.h game.cpp game.h player_stats.cpp player_stats.h)
add_executable(DaburuJanpuServer ${SOURCE_FILES})