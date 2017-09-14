cmake_minimum_required(VERSION 3.7)
project(GeradorDeGrafos)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        MainGeradorToGraphviz.c
        utarray.h
        uthash.h
        utlist.h
        utringbuffer.h
        utstring.h)

add_executable(GeradorDeGrafos ${SOURCE_FILES})