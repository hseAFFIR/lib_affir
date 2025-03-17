#ifndef LIB_AFFIR_TOKEN_H
#define LIB_AFFIR_TOKEN_H

#include <iostream>
#include <string>
#include "../common.h"

/**
 * @class Token
 * @brief Класс, представляющий токен в тексте.
 */
class Token {
private:
    std::string body; ///< Слово, которое представляет токен.
    Pos pos; ///< Позиция токена в тексте.
    Pos index; ///< Позиция токена среди всех токенов.
    FileId fileId; ///< Идентификатор файла, содержащего токен.

public:
    /**
     * @brief Конструктор по умолчанию.
     * Инициализирует пустой токен с позицией 0, индексом 0 и fileId 0.
     */
    Token() : pos(0), index(0), fileId(0) {
    }

    /**
     * @brief Параметризованный конструктор.
     * @param body Содержимое токена (слово).
     * @param pos Позиция токена в тексте.
     * @param index Позиция среди токенов.
     * @param fileId Идентификатор файла.
     */
    Token(const std::string &body, Pos pos, Pos index, FileId fileId)
        : body(body), pos(pos), index(index), fileId(fileId) {
    }

    /**
     * @brief Получает содержимое токена.
     * @return Строка, содержащая текст токена.
     */
    std::string getBody() const;

    /**
     * @brief Получает позицию токена в тексте.
     * @return Позиция токена.
     */
    Pos getPos() const;

    /**
     * @brief Получает позицию токена среди других токенов.
     * @return Индекс токена.
     */
    Pos getIndex() const;

    /**
     * @brief Получает идентификатор файла, содержащего токен.
     * @return Идентификатор файла.
     */
    FileId getFileId() const;

    /**
     * @brief Устанавливает новое содержимое токена.
     * @param newBody Новое значение для токена.
     */
    void setBody(const std::string &newBody);

    /**
     * @brief Устанавливает новую позицию токена в тексте.
     * @param newPos Новая позиция.
     */
    void setPos(Pos newPos);

    /**
     * @brief Устанавливает новый индекс токена среди других токенов.
     * @param newIndex Новый индекс.
     */
    void setIndex(Pos newIndex);

    /**
     * @brief Перегруженный оператор сравнения.
     * @param other Другой объект Token для сравнения.
     * @return true, если токены равны, иначе false.
     */
    bool operator==(const Token &other) const;
};

/**
 * @brief Перегрузка оператора вывода в поток.
 * @param os Поток вывода.
 * @param token Токен, который будет выведен в поток.
 * @return Поток вывода.
 */
std::ostream &operator<<(std::ostream &os, const Token &token);

#endif // LIB_AFFIR_TOKEN_H
