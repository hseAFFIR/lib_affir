#ifndef LIB_TESTS_COMMON_H
#define LIB_TESTS_COMMON_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <unordered_map>

#include "../src/tokenizer/filters/filters.h"
#include "../src/processing/dataHandler.h"
#include "../src/storages/indexes/multi/multiFileIndexStorage.h"
#include "../src/storages/indexes/iIndexStorage.h"
#include "../src/indexer/indexer.h"
#include "../src/common.h"

namespace fs = std::filesystem;

#endif //LIB_TESTS_COMMON_H
