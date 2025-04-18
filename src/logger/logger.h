#pragma once // Или используйте #ifndef/#define include guards

#include "quill/Logger.h"
#include "quill/Frontend.h"
#include "quill/LogMacros.h" // Включаем макросы для удобства
#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/LogMacros.h"
#include "quill/Logger.h"
#include "quill/sinks/ConsoleSink.h"
#include <string_view>


inline quill::Logger* GetRootLogger() {
    return quill::Frontend::get_logger("root");
}
class Logger {
public:
    static void init(std::string level);

private:
    static quill::Logger* logger;
};


// Опционально: Определите свои макросы для еще большего удобства,
// чтобы не передавать логгер каждый раз
#define MY_LOG_INFO(...) LOG_INFO(GetRootLogger(), __VA_ARGS__)
#define MY_LOG_ERROR(...) LOG_ERROR(GetRootLogger(), __VA_ARGS__)
#define MY_LOG_WARN(...) LOG_WARN(GetRootLogger(), __VA_ARGS__)
// и т.д. для других уровней