cmake_minimum_required(VERSION 3.31)
project(hw3 C CXX)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(hw3
        main.c
        main.h
        stack.c
        stack.h
        ackermann.c
        ackermann.h
        linked_list.c
        linked_list.h
        fun.c
        fun.h)
