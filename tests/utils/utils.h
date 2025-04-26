#ifndef LIB_TESTS_UTILS_H
#define LIB_TESTS_UTILS_H

#include "../common.h"
#include "../../affir/engine.h"
#include "../../affir/common.h"

using namespace affir;

void printPeakMemoryUsage();

size_t countOccurrences(const std::string &text, const std::string &word);

void searchWordInFiles(const std::string &folderPath, const std::string &word);

size_t getCurrenMemoryUsage();

std::string printIndStorageType(IndexStorageType &indexStorageType);

void deleteStorageForTests();

std::string filterTypeToString(FilterType fT);

const std::string indexDirPath = "index_files";
const std::string fileStorageMeta = "file_storage_metadata.bin";
const std::string fileStorage = "file_storage_raw";

#endif //LIB_TESTS_UTILS_H
