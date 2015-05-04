cmake_minimum_required(VERSION 2.8)

project(libcutil C)

add_definitions(-std=c99 -Wall -static -D_GNU_SOURCE -g)

add_library(cutil STATIC
    alloc.c
    log.c
    test.c
)

add_subdirectory(tests)
