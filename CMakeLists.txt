cmake_minimum_required(VERSION 3.7)
project(Convert)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES src/ConvertTo.c)
add_executable(Convert ${SOURCE_FILES})