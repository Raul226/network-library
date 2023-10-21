cmake_minimum_required(VERSION 3.6)
project(network VERSION 1.0)

add_library(network server.cpp client.cpp connection.cpp datagram.cpp)
if(WIN32)
    target_link_libraries(network PUBLIC Ws2_32.lib)
endif()