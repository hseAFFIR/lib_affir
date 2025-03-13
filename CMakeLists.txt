cmake_minimum_required(VERSION 3.29)
project(lib_affir)

set(CMAKE_CXX_STANDARD 20)

set(MODEL_FILES
        src/models/token.h
        src/models/token.cpp
        src/models/bigToken.h
        src/models/bigToken.cpp
)

set(INDEXER_FILES
        src/indexer/indexer.cpp
        src/indexer/indexer.h
)

set(SOURCE_FILES
        ${MODEL_FILES}
        ${INDEXER_FILES}
        src/main.cpp
)

add_executable(lib_affir ${SOURCE_FILES})

