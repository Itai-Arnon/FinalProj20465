cmake_minimum_required(VERSION 3.22.1)
project(mReloaded14 C)

set(CMAKE_C_STANDARD 90)

include_directories(.)

add_executable(mReloaded14
        src/macro.c
        include/macro.h
        include/global_vars.h
        include/macro_list.h
        src/macro_list.c
        include/shared.h
        src/utils.c
        include/utils.h
        src/assembler.c
        include/assembler.h
        src/parser.c
        include/parser.h
        include/symbols.h
        src/symbols.c
        src/first_pass.c
        include/first_pass.h
        src/second_pass.c
        include/second_pass.h
)
