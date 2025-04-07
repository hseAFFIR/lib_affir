cmake_minimum_required(VERSION 3.28.1)
project(lib_affir)

set(CMAKE_CXX_STANDARD 20)

set(STORAGE_FILES
        src/storages/files/fileStorage.cpp
        src/storages/files/fileStorage.h
        src/storages/indexes/iIndexStorage.h
        src/storages/indexes/multi/multiFileIndexStorage.h
        src/storages/indexes/multi/multiFileIndexStorage.cpp
)

set(MODEL_FILES
        src/models/token.h
        src/models/token.cpp
        src/models/bigToken.h
        src/models/bigToken.cpp
)

set(PROCESSING_FILES
        src/processing/filterOrder.h
        src/processing/dataHandler.h
        src/processing/dataHandler.cpp
)

set(INDEXER_FILES
        src/indexer/indexer.cpp
        src/indexer/indexer.h
)

set(SEARCHER_FILES
        src/searcher/search.cpp
        src/searcher/search.h
)

set(FILTERS
        src/tokenizer/filters/base.h
        src/tokenizer/filters/stemFilter.cpp
        src/tokenizer/filters/stemFilter.h
        src/tokenizer/filters/russianPorterStemmer.cpp
        src/tokenizer/filters/russianPorterStemmer.h
        src/tokenizer/filters/englishStemmer.cpp
        src/tokenizer/filters/englishStemmer.h
        src/tokenizer/filters/htmler.cpp
        src/tokenizer/filters/htmler.h
        src/tokenizer/filters/lowercaser.cpp
        src/tokenizer/filters/lowercaser.h
        src/tokenizer/filters/punctuator.h
        src/tokenizer/filters/punctuator.cpp
        src/tokenizer/filters/stopWords.h
        src/tokenizer/filters/stopWords.cpp
        src/tokenizer/tokenizer.h
        src/tokenizer/filters/filters.h)

set(SOURCE_FILES
        ${STORAGE_FILES}
        ${MODEL_FILES}
        ${FILTERS}
        ${INDEXER_FILES}
        ${PROCESSING_FILES}
        ${SEARCHER_FILES}
        src/main.cpp
        src/common.h
        src/logger/logger.h
        src/logger/logger.cpp
        src/tokenizer/Tokenizer.cpp
        src/tokenizer/Tokenizer.h)


add_executable(lib_affir ${SOURCE_FILES})

add_subdirectory(externals/spdlog)
target_link_libraries(lib_affir PRIVATE spdlog::spdlog)
