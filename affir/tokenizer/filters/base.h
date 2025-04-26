#ifndef BASE_H
#define BASE_H

#include <string>
#include "filterOrder.h"

namespace affir {
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
        virtual void process(std::string &token) = 0;

        virtual FilterOrder getOrder() const = 0;
    };
}
#endif // BASE_H
