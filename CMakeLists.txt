cmake_minimum_required(VERSION 3.6)
project(network VERSION 1.0)

add_library(network src/server.cpp src/client.cpp src/connection.cpp src/datagram.cpp)
target_include_directories(network PUBLIC src/)

if(WIN32)
    target_link_libraries(network PUBLIC Ws2_32.lib)
endif()