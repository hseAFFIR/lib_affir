#include "Tokenizer.h"
#include <cctype>
#include <iostream>

/**
 * @brief Конструктор Tokenizer.
 * @param filters Вектор указателей на объекты фильтров, применяемых к токенам.
 */
Tokenizer::Tokenizer(std::vector<Base*> filters)
    : filters(std::move(filters)), htmlPattern(R"(<\/?\w+.*?>)") {}

/**
 * @brief Проверяет, является ли символ кириллическим (UTF-8).
 */
bool isCyrillicChar(const std::string& text, size_t index) {
    if (index + 1 >= text.size()) return false;

    unsigned char first = text[index];
    unsigned char second = text[index + 1];

    if (first == 0xD0) {
        return (second >= 0x90 && second <= 0xBF);
    } else if (first == 0xD1) {
        return (second >= 0x80 && second <= 0x8F);
    }

    return false;
}

/**
 * @brief Проверяет, является ли символ буквой или цифрой (ASCII или кириллица).
 */
bool isAlnumCustom(const std::string& text, size_t index) {
    unsigned char ch = text[index];

    // ASCII буквы и цифры
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')) {
        return true;
    }

    // Кириллица
    return isCyrillicChar(text, index);
}

/**
 * @brief Разбивает текст на токены без применения фильтров.
 * @param text Входной текст.
 * @param fileId Идентификатор файла, откуда взят текст.
 * @param callback Функция обратного вызова для обработки каждого найденного токена.
 */
void Tokenizer::tokenizeRaw(const std::string &text, std::function<void(Token)> callback, std::optional<FileId> fileId) {
    size_t currentPos = 0;
    size_t index = 0;
    size_t i = 0;
    FileId actualFileId = fileId.value_or(0);

    while (i < text.size()) {
        // Пропускаем пробельные символы
        while (i < text.size() && std::isspace(static_cast<unsigned char>(text[i]))) {
            ++i;
            ++currentPos;
        }

        if (i >= text.size()) break;

        size_t startPos = currentPos;
        std::string token;

        // Проверяем на HTML-тег
        if (text[i] == '<') {
            std::smatch match;
            std::string remainingText = text.substr(i);

            if (std::regex_search(remainingText, match, htmlPattern) && match.position() == 0) {
                token = match.str();
                i += match.length();
                currentPos += match.length();
            } else {
                token = text[i];
                ++i;
                ++currentPos;
            }
        }
        // Одиночные символы (не буква/цифра)
        else if (!isAlnumCustom(text, i)) {
            unsigned char ch = text[i];
            token = ch;
            ++i;
            ++currentPos;
        }
        // Собираем слова и числа (ASCII + кириллица)
        else {
            while (i < text.size() && isAlnumCustom(text, i)) {
                unsigned char ch = text[i];
                token += ch;

                // Если это кириллический символ, он состоит из двух байт
                if (isCyrillicChar(text, i)) {
                    token += text[i + 1];
                    i += 2;
                    currentPos++;
                } else {
                    ++i;
                    ++currentPos;
                }
            }
        }

        callback(Token(token, startPos, index++, actualFileId));
    }
}

/**
 * @brief Разбивает текст на токены с применением фильтров.
 * @param text Входной текст.
 * @param fileId Идентификатор файла.
 * @param callback Функция обратного вызова для обработки отфильтрованных токенов.
 */
void Tokenizer::tokenizeFiltered(const std::string &text, std::function<void(Token)> callback, std::optional<FileId> fileId) {
    tokenizeRaw(text, [this, &callback](Token token) {
        std::string filteredToken = applyFilters(token.getBody());
        if (!filteredToken.empty()) {
            callback(Token(filteredToken, token.getPos(), token.getIndex(), token.getFileId()));
        }
    }, fileId);
}

/**
 * @brief Применяет фильтры к токену.
 * @param token Исходный токен.
 * @return Отфильтрованный токен или пустая строка.
 */
std::string Tokenizer::applyFilters(const std::string &token) {
    std::string *result = const_cast<std::string*>(&token);

    for (const auto &filter : filters) {
        *result = filter->process(*result);
        if (result->empty()) break;
    }

    return *result;
}
