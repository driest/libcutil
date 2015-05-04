cmake_minimum_required(VERSION 2.8)

project(libcutil C)

add_definitions(-O3 -std=c99 -Wall -static -D_GNU_SOURCE -Wno-deprecated-declarations)

add_library(cutil STATIC
    log.c
    test.c
)

add_subdirectory(tests)
