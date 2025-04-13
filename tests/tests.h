//
// Created by Shon on 11.04.2025.
//

#ifndef LIB_TESTS_TESTS_H
#define LIB_TESTS_TESTS_H

#include "common.h"

void runTestGetTokenInCreatedIndex(std::string &text, size_t buffer, std::vector<Base *> &filters, IIndexStorage &storage);

void runTestTokIndIndStor(const std::string &folderPath, size_t buffer, std::vector<Base *> &filters,
                          IIndexStorage &storage);

void runTestProcessingWithoutReadTime(const std::string &folderPath, size_t buffer, std::vector<Base *> &filters,
                                      IIndexStorage &storage);

#endif //LIB_TESTS_TESTS_H
