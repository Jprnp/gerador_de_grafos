cmake_minimum_required(VERSION 3.8)
project(GeradorDeGrafos)

set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)
set(SOURCE_FILES main.c)

LINK_DIRECTORIES(C:/Users/Ullrick/CLionProjects/graphviz/cmake-build-debug)

add_executable(GeradorDeGrafos ${SOURCE_FILES})

target_link_libraries(GeradorDeGrafos libgraphviz.a)