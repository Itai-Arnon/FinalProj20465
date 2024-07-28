cmake_minimum_required(VERSION 3.28)
project(m14 C)

set(CMAKE_C_STANDARD 90)

include_directories(.)

add_executable(m14
        scripts/macro.c
        headers/macro.h
        headers/global_vars.h
        headers/linkedlist.h
        scripts/linkedlist.c
        headers/shared.h
        scripts/utils.c
        headers/utils.h
        scripts/assembler.c
        headers/assembler.h
        scripts/parser.c
        headers/parser.h
        headers/pass_one.h
        scripts/pass_one.c)
