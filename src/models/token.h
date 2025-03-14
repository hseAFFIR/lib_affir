#ifndef LIB_AFFIR_TOKEN_H
#define LIB_AFFIR_TOKEN_H

#include <iostream>
#include <string>
#include "../common.h"

class Token {
private:
    std::string body;  // Слово
    Pos pos;     // Позиция в тексте
    Pos index;   // Позиция среди токенов
    FileId fileId; //

public:
    // Конструкторы
    Token() : pos(0), index(0), fileId(0) { }

    Token(const std::string &body, Pos pos, Pos index, FileId fileId)
            : body(body), pos(pos), index(index), fileId(fileId) { }  // Параметризованный конструктор

    // Геттеры
    std::string getBody() const;

    Pos getPos() const;

    Pos getIndex() const;

    size_t getFileId() const;

    // Сеттеры
    void setBody(const std::string &newBody);

    void setPos(Pos newPos);

    void setIndex(Pos newIndex);

    // Перегрузка оператора сравнения
    bool operator==(const Token &other) const;
};

// Перегрузка оператора вывода в поток
std::ostream &operator<<(std::ostream &os, const Token &token);

#endif // LIB_AFFIR_TOKEN_H