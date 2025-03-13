#ifndef LIB_AFFIR_TOKEN_H
#define LIB_AFFIR_TOKEN_H

#include <iostream>
#include <string>

class Token {
private:
    std::string body;  // Слово
    int pos;           // Позиция в тексте
    int index;         // Позиция среди токенов

public:
    // Конструкторы
    Token();  // Конструктор по умолчанию
    Token(const std::string& body, int pos, int index);  // Параметризованный конструктор

    // Геттеры
    std::string getBody() const;
    int getPos() const;
    int getIndex() const;

    // Сеттеры
    void setBody(const std::string& newBody);
    void setPos(int newPos);
    void setIndex(int newIndex);

    // Перегрузка оператора сравнения
    bool operator==(const Token& other) const;
};

// Перегрузка оператора вывода в поток
std::ostream& operator<<(std::ostream& os, const Token& token);

#endif // LIB_AFFIR_TOKEN_H