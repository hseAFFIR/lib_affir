#include "logger.h"

quill::Logger* Logger::logger = nullptr;

void Logger::init(std::string level) {
    // Запуск backend потока
    quill::Backend::start();

    quill::Logger* logger = quill::Frontend::create_or_get_logger(
            "root", quill::Frontend::create_or_get_sink<quill::ConsoleSink>("sink_id_1"));

    // Устанавливаем уровень логирования
    if (level == "info")
        logger->set_log_level(quill::LogLevel::Info);
    if (level == "debug")
        logger->set_log_level(quill::LogLevel::Debug);
    if (level == "none")
        logger->set_log_level(quill::LogLevel::None);
    if (level == "error")
        logger->set_log_level(quill::LogLevel::Error);

}
