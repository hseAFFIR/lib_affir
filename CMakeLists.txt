cmake_minimum_required(VERSION 3.29)
project(lib_affir)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
        src/main.cpp
)

add_executable(lib_affir ${SOURCE_FILES})