//
// Created by amenk on 22.03.2025.
//
#include "logger.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <cstdlib> // для getenv

std::shared_ptr<spdlog::logger> Logger::logger = nullptr;

void Logger::init(const std::string& logFilePath) {

    // Создаем сенки (sinks) для вывода в файл и консоль
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);

    // Устанавливаем формат вывода
    console_sink->set_pattern("%^%l: %Y-%m-%d %H:%M:%S [%n]: %v%$");
    file_sink->set_pattern("%l: %Y-%m-%d %H:%M:%S [%n]: %v");

    // Создаем логгер с двумя сенками
    logger = std::make_shared<spdlog::logger>("logger", spdlog::sinks_init_list{console_sink, file_sink});

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