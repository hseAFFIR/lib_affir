cmake_minimum_required(VERSION 3.28.1)
project(lib_affir)

set(CMAKE_CXX_STANDARD 20)

set(STORAGE_FILES
        affir/storages/files/fileStorage.cpp
        affir/storages/files/fileStorage.h
        affir/storages/indexes/iIndexStorage.h
        affir/storages/indexes/multi/multiFileIndexStorage.h
        affir/storages/indexes/multi/multiFileIndexStorage.cpp
        affir/storages/indexes/single/singleIndexStorage.cpp
        affir/storages/indexes/single/singleIndexStorage.h
)

set(MODEL_FILES
        affir/models/bigToken.h
        affir/models/bigToken.cpp
)

set(PROCESSING_FILES
        affir/tokenizer/filters/filterOrder.h
        affir/processing/dataHandler.h
        affir/processing/dataHandler.cpp
)

set(INDEXER_FILES
        affir/indexer/indexer.cpp
        affir/indexer/indexer.h
)

set(SEARCHER_FILES
        affir/searcher/search.cpp
        affir/searcher/search.h
        affir/searcher/searchResult.h
)

set(FILTERS
        affir/tokenizer/filters/base.h
        affir/tokenizer/filters/stemFilter.cpp
        affir/tokenizer/filters/stemFilter.h
        affir/tokenizer/filters/russianPorterStemmer.cpp
        affir/tokenizer/filters/russianPorterStemmer.h
        affir/tokenizer/filters/englishStemmer.cpp
        affir/tokenizer/filters/englishStemmer.h
        affir/tokenizer/filters/htmler.cpp
        affir/tokenizer/filters/htmler.h
        affir/tokenizer/filters/lowercaser.cpp
        affir/tokenizer/filters/lowercaser.h
        affir/tokenizer/filters/punctuator.h
        affir/tokenizer/filters/punctuator.cpp
        affir/tokenizer/filters/stopWords.h
        affir/tokenizer/filters/stopWords.cpp
        affir/tokenizer/filters/filters.h)

set(SOURCE_FILES
        ${STORAGE_FILES}
        ${MODEL_FILES}
        ${FILTERS}
        ${INDEXER_FILES}
        ${PROCESSING_FILES}
        ${SEARCHER_FILES}
        affir/common.h
        affir/logger/logger.h
        affir/logger/logger.cpp
        affir/tokenizer/tokenizer.h
        affir/tokenizer/tokenizer.cpp
        affir/engine.cpp
        affir/engine.h)


add_executable(lib_affir main.cpp
        ${SOURCE_FILES})

add_executable(lib_tests tests/main.cpp
        tests/functional/withoutReadTime.cpp
        tests/common.h
        tests/functional/tokIndIndStor.cpp
        tests/functional/getTokenInCreatedIndex.cpp
        tests/functional/filtersTests.cpp
        tests/pipline/engineTests.cpp
        tests/pipline/groupTest.cpp
        tests/utils/utils.cpp
        tests/tests.h
        tests/utils/utils.h
        ${SOURCE_FILES}
)

set(CMAKE_PREFIX_PATH externals/quill_x64-windows)

find_package(quill REQUIRED)
target_link_libraries(lib_affir PRIVATE quill::quill)
target_link_libraries(lib_tests PRIVATE quill::quill)
