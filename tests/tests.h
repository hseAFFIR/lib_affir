//
// Created by Shon on 11.04.2025.
//

#ifndef LIB_TESTS_TESTS_H
#define LIB_TESTS_TESTS_H

#include "common.h"
#include "../src/engine.h"
#include "utils/utils.h"

void runTestGetTokenInCreatedIndex(std::string &text, size_t buffer, std::vector<Base *> &filters, IIndexStorage &storage);

void runTestTokIndIndStor(const std::string &folderPath, size_t buffer, std::vector<Base *> &filters,
                          IIndexStorage &storage);

void runTestProcessingWithoutReadTime(const std::string &folderPath, size_t buffer, std::vector<Base *> &filters,
                                      IIndexStorage &storage);

void runEngineTest(std::string dataPath, EngineFocus engineFocus, FilterType filterFlags, IndexStorageType indexStorageType, size_t buffer);

void runEngineSideLoadTest(std::string dataPath, EngineFocus engineFocus, FilterType filterFlags,
                           IndexStorageType indexStorageType, size_t buffer);

#endif //LIB_TESTS_TESTS_H
