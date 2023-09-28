cmake_minimum_required(VERSION 3.26)
project(myalloc)

set(CMAKE_CXX_STANDARD 17)

add_executable(myalloc main.c
        malloc.h
        malloc.c
)
