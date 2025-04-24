#include "tokenizer.h"
#include <cctype>
#include <iostream>

/**
 * @brief Проверяет, является ли символ кириллическим (UTF-8).
 */
bool isCyrillicChar(const std::string& text, size_t index) {
    if (index + 1 >= text.size()) return false;

    unsigned char first = text[index];
    unsigned char second = text[index + 1];

    if (first == 0xD0) {
        return (second >= 0x90 && second <= 0xBF)||second == 0x81;
    } else if (first == 0xD1) {
        return (second >= 0x80 && second <= 0x8F) || second == 0x91;
    }

    return false;
}

/**
 * @brief Проверяет, является ли символ буквой или цифрой (ASCII или кириллица).
 */
bool isAlnumCustom(const std::string& text, size_t index) {
    unsigned char ch = text[index];

    // ASCII буквы и цифры
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'))
        return true;

    // Кириллица
    return isCyrillicChar(text, index);
}

bool Tokenizer::hasNext() {
    // Пропускаем пробельные символы
    while (i < text.size() && std::isspace(static_cast<unsigned char>(text[i]))) {
        ++i;
        ++currentPos;
    }

    // Skip empty tokens in order to make next token valid
    while(i < text.size() and !prepareNext());

    return i < text.size();
}

bool Tokenizer::prepareNext() {
    const size_t startPos = currentPos;
    std::string body;

    // Проверяем на HTML-тег
    if (text[i] == '<') {
        std::smatch match;
        std::string remainingText = text.substr(i, htmlPatternLimit);

        if (std::regex_search(remainingText, match, htmlPattern) && match.position() == 0) {
            body = match.str();
            i += match.length();
            currentPos += match.length();
        } else {
            body = text[i];
            ++i;
            ++currentPos;
        }
    }
    // Одиночные символы (не буква/цифра)
    else if (!isAlnumCustom(text, i)) {
        body = text[i];
        ++i;
        ++currentPos;
    }
    // Собираем слова и числа (ASCII + кириллица)
    else {
        while (i < text.size() && isAlnumCustom(text, i)) {
            body += text[i];

            // Если это кириллический символ, он состоит из двух байт
            if (isCyrillicChar(text, i)) {
                body += text[i + 1];
                i += 2;
                currentPos++;
            } else {
                ++i;
                ++currentPos;
            }
        }
    }

    applyFilters(body);
    if (body.empty())
        return false;
    preparedToken = Token(body, {startPos, wordPos++}, fileId);
    return true;
}

Token Tokenizer::next() {
    return std::move(preparedToken);
}

/**
 * @brief Разбивает текст на токены без применения фильтров.
 * @param text Входной текст.
 * @param fileId Идентификатор файла, откуда взят текст.
 * @param callback Функция обратного вызова для обработки каждого найденного токена.
 */
void Tokenizer::tokenize(std::string &input, FileId inFileId) {
    currentPos = 0;
    wordPos = 0;
    i = 0;
    text = std::move(input);
    fileId = inFileId;
}

/**
 * @brief Применяет фильтры к токену.
 * @param token Исходный токен.
 * @return Отфильтрованный токен или пустая строка.
 */
void Tokenizer::applyFilters(std::string &token) {
    for (const auto &filter : filters) {
        filter->process(token);
        if (token.empty()) break;
    }
}
