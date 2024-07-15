cmake_minimum_required(VERSION 3.28)
project(m14 C)

set(CMAKE_C_STANDARD 90)

include_directories(.)

add_executable(m14
        macro.c
        macro.h
        global_vars.h)
