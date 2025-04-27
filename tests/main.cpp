#include <vector>
#include "../affir/storages/indexes/multi/multiFileIndexStorage.h"
#include "../affir/storages/indexes/single/singleIndexStorage.h"
#include "utils/utils.h"
#include "tests.h"
#include "../affir/logger/logger.h"

using namespace affir;

int main() {
    system("chcp 65001"); // Для корректного отображения русского языка в логах
    Logger::init(Logger::Level::None, "logs/test.log");

    LOG_INFO(Logger::logger, "Application started");

    groupTest("final_dataset");

    printPeakMemoryUsage();

    return 0;
}
