cmake_minimum_required(VERSION 3.6)
project(TestProject1 VERSION 1.0)

add_executable(TestProject1 main.cpp)
add_subdirectory("${CMAKE_SOURCE_DIR}/network")
target_include_directories(TestProject1 PUBLIC "${CMAKE_SOURCE_DIR}/network")
target_link_libraries(TestProject1 PUBLIC network)