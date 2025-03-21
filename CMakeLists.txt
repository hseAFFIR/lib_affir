cmake_minimum_required(VERSION 3.29)
project(lib_affir)

set(CMAKE_CXX_STANDARD 20)

set(STORAGE_FILES
        src/storages/fileStorage.cpp
        src/storages/fileStorage.h)

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

set(FILTERS
        src/tokenizer/filters/base.h
        src/tokenizer/filters/stemFilter.cpp
        src/tokenizer/filters/stemFilter.h
        src/tokenizer/filters/russianPorterStemmer.cpp
        src/tokenizer/filters/russianPorterStemmer.h
        src/tokenizer/filters/englishStemmer.cpp
        src/tokenizer/filters/englishStemmer.h)

set(SOURCE_FILES
        ${STORAGE_FILES}
        ${MODEL_FILES}
#        ${FILTERS}
        ${INDEXER_FILES}
        src/main.cpp
        src/common.h
)

add_executable(lib_affir ${SOURCE_FILES})