#pragma once // Или используйте #ifndef/#define include guards

#include "quill/Logger.h"
#include "quill/Frontend.h"
#include "quill/LogMacros.h" // Включаем макросы для удобства

// Функция, которая возвращает указатель на нужный логгер
inline quill::Logger* GetRootLogger() {
    // quill::Frontend::get_logger() обычно потокобезопасен
    // и возвращает один и тот же указатель для одного имени.
    // Если вы хотите гарантировать вызов get_logger только один раз
    // (хотя это обычно не нужно), можно использовать static:
    // static quill::Logger* logger = quill::Frontend::get_logger("root");
    // return logger;
    // Но обычно достаточно просто вернуть результат вызова:
    return quill::Frontend::get_logger("root");
}

// Опционально: Определите свои макросы для еще большего удобства,
// чтобы не передавать логгер каждый раз
#define MY_LOG_INFO(...) LOG_INFO(GetRootLogger(), __VA_ARGS__)
#define MY_LOG_ERROR(...) LOG_ERROR(GetRootLogger(), __VA_ARGS__)
#define MY_LOG_WARN(...) LOG_WARN(GetRootLogger(), __VA_ARGS__)
// и т.д. для других уровней