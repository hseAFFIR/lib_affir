cmake_minimum_required(VERSION 3.29)
project(lib_affir)

set(CMAKE_CXX_STANDARD 20)

set(STORAGE_FILES
        src/storages/fileStorage.cpp
        src/storages/fileStorage.h)

set(MODEL_FILES
        src/models/token.h
        src/models/token.cpp)

set(SOURCE_FILES
        ${STORAGE_FILES}
        ${MODEL_FILES}
        src/main.cpp
)

add_executable(lib_affir ${SOURCE_FILES})