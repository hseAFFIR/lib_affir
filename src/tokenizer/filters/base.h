//
// Created by okosh on 14.03.2025.
//

#ifndef BASE_H
#define BASE_H

#include <string>
#include "../../processing/filterOrder.h"

/**
 * @class Base
 * @brief Абстрактный базовый класс с виртуальным методом process.
 */
class Base {
public:
    /**
     * @brief Виртуальный деструктор.
     * Позволяет корректно удалять объекты производных классов через указатель на базовый класс.
     */
    virtual ~Base() = default;

    /**
     * @brief Чистый виртуальный метод для обработки токена.
     * Должен быть переопределён в производных классах.
     * @param token Входная строка, представляющая токен.
     * @return Строка, содержащая результат обработки.
     */
    virtual std::string process(const std::string &token) const = 0;

    virtual FilterOrder getOrder() const = 0;
};

#endif // BASE_H
