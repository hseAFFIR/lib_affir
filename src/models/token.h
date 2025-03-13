#ifndef LIB_AFFIR_TOKEN_H
#define LIB_AFFIR_TOKEN_H

#include <iostream>
#include <string>

class Token {
private:
    std::string body;  // Слово
    long long pos;     // Позиция в тексте
    long long index;   // Позиция среди токенов
    long long fileId; //

public:
    // Конструкторы
    Token();  // Конструктор по умолчанию
    Token(const std::string& body, long long pos, long long index, long long fileId);  // Параметризованный конструктор

    // Геттеры
    std::string getBody() const;
    long long getPos() const;
    long long getIndex() const;
    long long getFileId() const;

    // Сеттеры
    void setBody(const std::string& newBody);
    void setPos(long long newPos);
    void setIndex(long long newIndex);

    // Перегрузка оператора сравнения
    bool operator==(const Token& other) const;
};

// Перегрузка оператора вывода в поток
std::ostream& operator<<(std::ostream& os, const Token& token);

#endif // LIB_AFFIR_TOKEN_H