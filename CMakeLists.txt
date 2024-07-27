cmake_minimum_required(VERSION 3.28)
project(m14 C)

set(CMAKE_C_STANDARD 90)

include_directories(.)

add_executable(m14
        macro.c
        macro.h
        global_vars.h
        linkedlist.h
        linkedlist.c
        shared.h
        utils.c
        utils.h
        assembler.c
        assembler.h
        error.c
        error.h
        front.c
        front.h)
