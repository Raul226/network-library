cmake_minimum_required(VERSION 3.6)
project(demo VERSION 1.0)

add_executable(demo main.cpp)

add_subdirectory("${CMAKE_SOURCE_DIR}/network")
target_include_directories(demo PUBLIC "${CMAKE_SOURCE_DIR}/network")
target_link_libraries(demo PUBLIC network)