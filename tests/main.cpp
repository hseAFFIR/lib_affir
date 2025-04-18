#include <vector>

#include "../src/tokenizer/filters/filters.h"
#include "../src/processing/dataHandler.h"
#include "../src/storages/indexes/multi/multiFileIndexStorage.h"
#include "../src/storages/indexes/single/singleIndexStorage.h"
#include "utils/utils.h"
#include "tests.h"


#include "../src/logger/logger.h"

int main() {
    system("chcp 65001"); // Для корректного отображения русского языка в логах
      Logger::init("info");

//    LOG_INFO(logger, "Hello from {}!", std::string_view{"Quill"});
//    quill::Logger* logger = quill::Frontend::get_logger("root");
//    LOG_INFO(logger, "Logging something from {}", "someFunction");
//    LOG_INFO(LoggerWrapper::getInstance().getLogger(), "Logging something from {}", "someFunction");

    std::vector<Base*> filters = { new Htmler(), new Punctuator(),
                                   new StopWords()};


    MultiFileIndexStorage mfis;

    runTestProcessingWithoutReadTime("C:\\rep\\lib_affir\\tests\\death_data", 1024*1024*16, filters, mfis);

    printPeakMemoryUsage();

    for (auto f : filters) delete f;

    return 0;
}
