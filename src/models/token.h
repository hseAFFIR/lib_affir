#ifndef LIB_AFFIR_TOKEN_H
#define LIB_AFFIR_TOKEN_H

#include <iostream>
#include <string>

class Token {
private:
    std::string body;  // Слово
    unsigned long pos;     // Позиция в тексте
    unsigned long index;   // Позиция среди токенов
    unsigned long fileId; //

public:
    // Конструкторы
    Token();  // Конструктор по умолчанию
    Token(const std::string& body, unsigned long pos, unsigned long index, unsigned long fileId);  // Параметризованный конструктор

    // Геттеры
    std::string getBody() const;
    unsigned long getPos() const;
    unsigned long getIndex() const;
    unsigned long getFileId() const;

    // Сеттеры
    void setBody(const std::string& newBody);
    void setPos(unsigned long newPos);
    void setIndex(unsigned long newIndex);

    // Перегрузка оператора сравнения
    bool operator==(const Token& other) const;
};

// Перегрузка оператора вывода в поток
std::ostream& operator<<(std::ostream& os, const Token& token);

#endif // LIB_AFFIR_TOKEN_H