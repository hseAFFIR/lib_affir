#include "logger.h"

quill::Logger* Logger::logger = nullptr;

void Logger::init(std::string level, std::string path) {
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

    LOG_INFO(logger, "Logger module init, log level = {}", level);
    
    
    // Устанавливаем уровень логирования
    if (level == "info")
        logger->set_log_level(quill::LogLevel::Info);
    if (level == "debug")
        logger->set_log_level(quill::LogLevel::Debug);
    if (level == "error")
        logger->set_log_level(quill::LogLevel::Error);
    if (level == "warn")
        logger->set_log_level(quill::LogLevel::Warning);
    if (level == "none")
        logger->set_log_level(quill::LogLevel::None);
}