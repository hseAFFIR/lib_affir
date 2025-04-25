#include "logger.h"
#include <iostream>

quill::Logger* Logger::logger = nullptr;


void Logger::init(Level level, std::string path) {
    if (logger != nullptr) {
        LOG_WARNING(logger, "Logger is already initialized");
        return;
    }
    try{ 
    quill::Backend::start();

    std::vector<std::shared_ptr<quill::Sink>> sinks;
    sinks.push_back(quill::Frontend::create_or_get_sink<quill::ConsoleSink>("consoleSink"));

    if (!path.empty()) {
        auto file_sink = quill::Frontend::create_or_get_sink<quill::FileSink>(
            path,
            []() {
                quill::FileSinkConfig cfg;
                cfg.set_open_mode('w');
                cfg.set_filename_append_option(quill::FilenameAppendOption::StartDateTime);
                return cfg;
            }(),
            quill::FileEventNotifier{});
        sinks.push_back(file_sink);
    }

    // Создаем логгер с обоими синками
    logger = quill::Frontend::create_or_get_logger("root", sinks);

    LOG_INFO(logger, "Logger module init");
    
    
    // Устанавливаем уровень логирования
    switch (level) {
        case Level::Info:
            logger->set_log_level(quill::LogLevel::Info);
            break;
        case Level::Debug:
            logger->set_log_level(quill::LogLevel::Debug);
            break;
        case Level::Error:
            logger->set_log_level(quill::LogLevel::Error);
            break;
        case Level::Warn:
            logger->set_log_level(quill::LogLevel::Warning);
            break;
        case Level::None:
            logger->set_log_level(quill::LogLevel::None);
            break;
    }
}catch (const std::exception& e) {
        std::cerr << "Error initializing logger: " << e.what() << std::endl;
    }
}
Logger::~Logger() {
    quill::Frontend::remove_logger(logger);
}