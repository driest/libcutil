cmake_minimum_required(VERSION 2.8)

include_directories("..")

add_executable(cutil_tests tests_main.c)

target_link_libraries(cutil_tests cutil)
