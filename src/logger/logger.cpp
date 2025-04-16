//
// Created by amenk on 22.03.2025.
//
#include "logger.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>
#include <cstdlib> // для getenv

std::shared_ptr<spdlog::logger> Logger::logger = nullptr;

void Logger::init(const std::string& logFilePath) {

    spdlog::init_thread_pool(8192, 1);

    // Создаем сенки (sinks) для вывода в файл и консоль
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);
//    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logFilePath, 1024*1024*10, 3);
    std::initializer_list<spdlog::sink_ptr> sinks {file_sink, console_sink};


    // Устанавливаем формат вывода
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] %v");

    logger = std::make_shared<spdlog::async_logger>("logger", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);

    // Устанавливаем уровень логирования в зависимости от переменной окружения DEBUG
    const char* debugEnv = std::getenv("DEBUG");
    if (debugEnv && std::string(debugEnv) == "false") {
        logger->set_level(spdlog::level::info); // Только info и выше
    } else {
        logger->set_level(spdlog::level::trace); // Все уровни
    }

    // Регистрируем логгер
    spdlog::register_logger(logger);
}