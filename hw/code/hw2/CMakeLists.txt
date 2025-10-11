cmake_minimum_required(VERSION 3.31)
project(hw2 C CXX)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(hw2
        linked_list.c
        linked_list.h
        main.c
        main.h)
