#include <iostream>
#include "tokenizer/filters/filters.h"
#include "processing/dataHandler.h"
#include "storages/indexes/multi/multiFileIndexStorage.h"
#include "storages/indexes/single/singleIndexStorage.h"
#include <vector>
#include "logger/logger.h"
#include "tests/tests.cpp"

#include <cassert>
#include <fstream>

int main(){
    system("chcp 65001"); // Для корректного отображения русского языка в логах
    Logger::init("logs/log.txt");
    Logger::info("Main", "Application started");
    std::vector<Base*> filters = { new Htmler(), new Punctuator(),
        new StopWords()};


    MultiFileIndexStorage mfis;

    runTestProcessingWithoutReadTime("test_data", 1024*1024*16, filters, mfis);

    printPeakMemoryUsage();

    for (auto f : filters) delete f;

    return 0;
}
