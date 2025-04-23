#ifndef LIB_TESTS_UTILS_H
#define LIB_TESTS_UTILS_H

#include "../common.h"

void printPeakMemoryUsage();

size_t countOccurrences(const std::string &text, const std::string &word);

void searchWordInFiles(const std::string &folderPath, const std::string &word);

size_t getCurrenMemoryUsage();


#endif //LIB_TESTS_UTILS_H
