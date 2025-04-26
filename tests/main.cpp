#include <vector>
#include "../affir/storages/indexes/multi/multiFileIndexStorage.h"
#include "../affir/storages/indexes/single/singleIndexStorage.h"
#include "utils/utils.h"
#include "tests.h"
#include "../affir/logger/logger.h"

using namespace affir;

int main() {
    system("chcp 65001"); // Для корректного отображения русского языка в логах
    Logger::init(Logger::Level::Info, "logs/test.log");

    LOG_INFO(Logger::logger, "Application started");

    runEngineSideLoadTest("death_data", EngineFocus::POSES, FilterType::NONE, IndexStorageType::MULTI, 1024*1024*16);

    printPeakMemoryUsage();

    return 0;
}
