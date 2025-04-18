#include <vector>

#include "../src/tokenizer/filters/filters.h"
#include "../src/processing/dataHandler.h"
#include "../src/storages/indexes/multi/multiFileIndexStorage.h"
#include "../src/storages/indexes/single/singleIndexStorage.h"
#include "utils/utils.h"
#include "tests.h"

#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/LogMacros.h"
#include "quill/Logger.h"
#include "quill/sinks/ConsoleSink.h"
#include <string_view>

#include "../src/logger/logger.h"

int main() {
    system("chcp 65001"); // Для корректного отображения русского языка в логах
//    Logger::init("logs/test.txt");
//    auto async_logger = spdlog::basic_logger_mt<spdlog::async_factory>("async_log", "logs/async.txt");

//    Logger::info("Tests", "Application started");
//        Logger("test.txt");

//    Logger::info("Hello from logger.");
//    Logger::init();
//    Logger::log(213);

// Работает с фундаментальными типами
//    Logger::log_str("sadfwe");

    quill::Backend::start();

    quill::Logger* logger = quill::Frontend::create_or_get_logger(
            "root", quill::Frontend::create_or_get_sink<quill::ConsoleSink>("sink_id_1"));


    logger->set_log_level(quill::LogLevel::Info);
    LOG_INFO(logger, "Hello from {}!", std::string_view{"Quill"});
//    quill::Logger* logger = quill::Frontend::get_logger("root");

    LOG_INFO(logger, "Logging something from {}", "someFunction");
//    LOG_INFO(LoggerWrapper::getInstance().getLogger(), "Logging something from {}", "someFunction");

    std::vector<Base*> filters = { new Htmler(), new Punctuator(),
                                   new StopWords()};


    MultiFileIndexStorage mfis;

    runTestProcessingWithoutReadTime("C:\\rep\\lib_affir\\tests\\death_data", 1024*1024*16, filters, mfis);

    printPeakMemoryUsage();

    for (auto f : filters) delete f;

    return 0;
}
