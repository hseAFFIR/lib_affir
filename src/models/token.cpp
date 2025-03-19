#include "token.h"

// Геттеры
std::string Token::getBody() const { return body; }

Pos Token::getPos() const { return pos; }

Pos Token::getIndex() const { return index; }

FileId Token::getFileId() const { return fileId; }

// Сеттеры
void Token::setBody(const std::string &newBody) { body = newBody; }

void Token::setPos(Pos newPos) { pos = newPos; }

void Token::setIndex(Pos newIndex) { index = newIndex; }

// Перегрузка оператора вывода в поток
std::ostream &operator<<(std::ostream &os, const Token &token) {
    os << "Token(body='" << token.getBody() << "', pos=" << token.getPos()
       << ", index=" << token.getIndex() << ", fileId=" << token.getFileId() << ")";
    return os;
}

// Перегрузка оператора сравнения
bool Token::operator==(const Token &other) const {
    return body == other.body && pos == other.pos && index == other.index;
}