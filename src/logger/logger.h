//
// Created by amenk on 22.03.2025.
//

#ifndef LIB_AFFIR_LOGGER_H
#define LIB_AFFIR_LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>
#include <string>

/**
 * @class Logger
 * @brief The Logger class provides logging functionality using spdlog.
 *
 * This class supports logging messages at different levels (trace, debug, info, warn, error, critical)
 * and outputs them to both the console and a log file. The log level can be controlled via the
 * DEBUG environment variable.
 */
class Logger {
public:
    /**
     * @brief Initializes the logger with a specified log file path.
     *
     * @param logFilePath The path to the log file. Defaults to "logs/log.txt".
     */
    static void init(const std::string& logFilePath = "logs/log.txt");

    /**
     * @brief Logs a trace-level message.
     *
     * @tparam Args Types of the message arguments.
     * @param className The name of the class generating the log message.
     * @param fmt The format string for the message.
     * @param args The arguments to format into the message.
     */
    template<typename... Args>
    static void trace(const std::string& className, spdlog::format_string_t<Args...> fmt, Args&&... args) {
        if (logger) logger->trace("[{}]: {}", className, fmt::format(fmt, std::forward<Args>(args)...));
    }

    /**
     * @brief Logs a debug-level message.
     *
     * @tparam Args Types of the message arguments.
     * @param className The name of the class generating the log message.
     * @param fmt The format string for the message.
     * @param args The arguments to format into the message.
     */
    template<typename... Args>
    static void debug(const std::string& className, spdlog::format_string_t<Args...> fmt, Args&&... args) {
        if (logger) logger->debug("[{}]: {}", className, fmt::format(fmt, std::forward<Args>(args)...));
    }

    /**
     * @brief Logs an info-level message.
     *
     * @tparam Args Types of the message arguments.
     * @param className The name of the class generating the log message.
     * @param fmt The format string for the message.
     * @param args The arguments to format into the message.
     */
    template<typename... Args>
    static void info(const std::string& className, spdlog::format_string_t<Args...> fmt, Args&&... args) {
        if (logger) logger->info("[{}]: {}", className, fmt::format(fmt, std::forward<Args>(args)...));
    }

    /**
     * @brief Logs a warn-level message.
     *
     * @tparam Args Types of the message arguments.
     * @param className The name of the class generating the log message.
     * @param fmt The format string for the message.
     * @param args The arguments to format into the message.
     */
    template<typename... Args>
    static void warn(const std::string& className, spdlog::format_string_t<Args...> fmt, Args&&... args) {
        if (logger) logger->warn("[{}]: {}", className, fmt::format(fmt, std::forward<Args>(args)...));
    }

    /**
     * @brief Logs an error-level message.
     *
     * @tparam Args Types of the message arguments.
     * @param className The name of the class generating the log message.
     * @param fmt The format string for the message.
     * @param args The arguments to format into the message.
     */
    template<typename... Args>
    static void error(const std::string& className, spdlog::format_string_t<Args...> fmt, Args&&... args) {
        if (logger) logger->error("[{}]: {}", className, fmt::format(fmt, std::forward<Args>(args)...));
    }

    /**
     * @brief Logs a critical-level message.
     *
     * @tparam Args Types of the message arguments.
     * @param className The name of the class generating the log message.
     * @param fmt The format string for the message.
     * @param args The arguments to format into the message.
     */
    template<typename... Args>
    static void critical(const std::string& className, spdlog::format_string_t<Args...> fmt, Args&&... args) {
        if (logger) logger->critical("[{}]: {}", className, fmt::format(fmt, std::forward<Args>(args)...));
    }

private:
    static std::shared_ptr<spdlog::logger> logger; ///< The spdlog logger instance.
};

#endif //LIB_AFFIR_LOGGER_H