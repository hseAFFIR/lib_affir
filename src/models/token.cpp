//
// Created by Shon on 26.02.2025.
//

#include "token.h"
#include "token.h"

// Реализация конструкторов
Token::Token() : body(""), pos(0), index(0), fileId(0){}

Token::Token(const std::string& body, long long pos, long long index,long long fileId)
    : body(body), pos(pos), index(index), fileId(fileId){}

// Геттеры
std::string Token::getBody() const { return body; }
long long Token::getPos() const { return pos; }
long long Token::getIndex() const { return index; }
long long Token::getFileId() const { return fileId; }

// Сеттеры
void Token::setBody(const std::string& newBody) { body = newBody; }
void Token::setPos(long long newPos) { pos = newPos; }
void Token::setIndex(long long newIndex) { index = newIndex; }

// Перегрузка оператора вывода в поток
std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "Token(body='" << token.getBody() << "', pos=" << token.getPos()
       << ", index=" << token.getIndex() <<", fileId="<< token.getFileId() <<")";
    return os;
}

// Перегрузка оператора сравнения
bool Token::operator==(const Token& other) const {
    return body == other.body && pos == other.pos && index == other.index;
}