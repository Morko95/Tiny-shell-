cmake_minimum_required(VERSION 3.16)

project(TinyShell)
project(TinyShell C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic -g)

add_executable(tsh
    main.c
    eval.c
    parser.c
    read_input.c
    eval.h
    parser.h
    read_input.h
    shell.h
)
