//
// Created by Shon on 26.02.2025.
//

#include "token.h"
#include "token.h"

// Реализация конструкторов
Token::Token() : body(""), pos(0), index(0) {}

Token::Token(const std::string& body, int pos, int index)
    : body(body), pos(pos), index(index) {}

// Геттеры
std::string Token::getBody() const { return body; }
int Token::getPos() const { return pos; }
int Token::getIndex() const { return index; }

// Сеттеров
void Token::setBody(const std::string& newBody) { body = newBody; }
void Token::setPos(int newPos) { pos = newPos; }
void Token::setIndex(int newIndex) { index = newIndex; }

// Перегрузка оператора вывода в поток
std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "Token(body='" << token.getBody() << "', pos=" << token.getPos()
       << ", index=" << token.getIndex() << ")";
    return os;
}

// Перегрузка оператора сравнения
bool Token::operator==(const Token& other) const {
    return body == other.body && pos == other.pos && index == other.index;
}