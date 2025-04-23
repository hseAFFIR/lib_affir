#include <vector>

#include "../src/tokenizer/filters/filters.h"
#include "../src/processing/dataHandler.h"
#include "../src/storages/indexes/multi/multiFileIndexStorage.h"
#include "../src/storages/indexes/single/singleIndexStorage.h"
#include "../src/logger/logger.h"

#include "utils/utils.h"
#include "tests.h"

int main() {
    system("chcp 65001"); // Для корректного отображения русского языка в логах
    Logger::init("logs/test.txt");
    Logger::info("Tests", "Application started");


    runEngineSideLoadTest("death_data", EngineFocus::POSES, FilterType::NONE, IndexStorageType::MULTI, 1024*1024*16);

    printPeakMemoryUsage();



    return 0;
}
