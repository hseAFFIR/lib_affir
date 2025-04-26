#ifndef LIB_TESTS_COMMON_H
#define LIB_TESTS_COMMON_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <unordered_map>

#include "../affir/tokenizer/filters/filters.h"
#include "../affir/processing/dataHandler.h"
#include "../affir/storages/indexes/multi/multiFileIndexStorage.h"
#include "../affir/storages/indexes/iIndexStorage.h"
#include "../affir/indexer/indexer.h"
#include "../affir/common.h"

namespace fs = std::filesystem;

#endif //LIB_TESTS_COMMON_H
