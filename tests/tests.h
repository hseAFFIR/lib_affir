//
// Created by Shon on 11.04.2025.
//

#ifndef LIB_TESTS_TESTS_H
#define LIB_TESTS_TESTS_H

#include "common.h"
#include "../affir/engine.h"
#include "utils/utils.h"

using namespace affir;

void runTestGetTokenInCreatedIndex(std::string &text, size_t buffer, std::vector<Base *> &filters, IIndexStorage &storage);

void runTestTokIndIndStor(const std::string &folderPath, size_t buffer, std::vector<Base *> &filters,
                          IIndexStorage &storage);

void runTestProcessingWithoutReadTime(const std::string &folderPath, size_t buffer, std::vector<Base *> &filters,
                                      IIndexStorage &storage);

void runEngineTestFile(std::string dataPath, EngineFocus engineFocus, FilterType filterFlags, IndexStorageType indexStorageType, size_t buffer);

void runEngineTestString(std::string dataPath, EngineFocus engineFocus, FilterType filterFlags,
                           IndexStorageType indexStorageType, size_t buffer);

void runEngieTestSearch(std::string dataPath, EngineFocus engineFocus, FilterType filterFlags,
                        IndexStorageType indexStorageType, size_t buffer);

void groupTest(std::string dataPath);

void filtersTests(std::string dataPath);

void saveEngineTestCsvToFile();

void saveFiltersTestCsvToFile();

#endif //LIB_TESTS_TESTS_H
