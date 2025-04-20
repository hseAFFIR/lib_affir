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

/**
 * @brief Logger class providing logging functionality using the Quill library
 * 
 * This class provides a singleton logger instance that can write logs to both
 * console and file simultaneously. It supports multiple log levels and
 * configurable output destinations.
 */
class Logger {
public:
    /**
     * @brief Initialize the logger with specified log level and file path
     * 
     * @param level The log level to set ("info", "debug", "error", "warn", "none")
     * @param path Optional path to the log file. If empty, only console logging is enabled
     */
    static void init(std::string level, std::string path);

    /**
     * @brief Get the root logger instance
     * 
     * @return quill::Logger* Pointer to the root logger instance
     */
    static quill::Logger* GetRootLogger() {
        return quill::Frontend::get_logger("root");
    }

private:
    static quill::Logger* logger;  ///< Static instance of the logger
};

#endif // LIB_AFFIR_LOGGER_H
