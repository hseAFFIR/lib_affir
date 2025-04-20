#ifndef LIB_AFFIR_LOGGER_H
#define LIB_AFFIR_LOGGER_H

#include "quill/Logger.h"
#include "quill/Frontend.h"
#include "quill/LogMacros.h"
#include "quill/Backend.h"
#include "quill/sinks/ConsoleSink.h"
#include "quill/sinks/FileSink.h"
#include <utility>
#include <string_view>

class Logger {
public:
    static void init(std::string level, std::string path);
    static quill::Logger* GetRootLogger() {
        return quill::Frontend::get_logger("root");
    }
private:
    static quill::Logger* logger;
};

#endif // LIB_AFFIR_LOGGER_H
