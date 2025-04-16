#include <vector>

#include "../src/tokenizer/filters/filters.h"
#include "../src/processing/dataHandler.h"
#include "../src/storages/indexes/multi/multiFileIndexStorage.h"
#include "../src/storages/indexes/single/singleIndexStorage.h"
#include "../src/logger/logger.h"

#include "utils/utils.h"
#include "tests.h"
#include "spdlog/async.h"

int main() {
    system("chcp 65001"); // Для корректного отображения русского языка в логах
//    Logger::init("logs/test.txt");
    auto async_logger = spdlog::basic_logger_mt<spdlog::async_factory>("async_log", "logs/async.txt");

    Logger::info("Tests", "Application started");



    std::vector<Base*> filters = { new Htmler(), new Punctuator(),
                                   new StopWords()};


    MultiFileIndexStorage mfis;

    runTestProcessingWithoutReadTime("C:\\rep\\lib_affir\\tests\\final_dataset", 1024*1024*16, filters, mfis);

    printPeakMemoryUsage();

    for (auto f : filters) delete f;

    return 0;
}
