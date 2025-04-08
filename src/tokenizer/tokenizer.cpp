#include "tokenizer.h"
#include <cctype>
#include <iostream>

/**
 * @brief Конструктор Tokenizer.
 * @param filters Вектор указателей на объекты фильтров, применяемых к токенам.
 */
Tokenizer::Tokenizer(std::vector<Base*> filters)
    : filters(std::move(filters)), htmlPattern(R"(<\/?\w+.*?>)") {}

/**
 * @brief Разбивает текст на токены без применения фильтров.
 * @param text Входной текст.
 * @param fileId Идентификатор файла, откуда взят текст.
 * @param callback Функция обратного вызова для обработки каждого найденного токена.
 */
void Tokenizer::tokenizeRaw(const std::string &text, FileId fileId, std::function<void(Token)> callback) {
    size_t currentPos = 0;
    size_t index = 0;

    auto it = text.begin();
    while (it != text.end()) {
        while (it != text.end() && std::isspace(*it)) {
            ++it;
            ++currentPos;
        }

        if (it == text.end()) break;

        size_t startPos = currentPos;
        std::string token;

        if (*it == '<') {  // Проверяем HTML-теги
            std::smatch match;
            std::string remainingText(it, text.end());

            if (std::regex_search(remainingText, match, htmlPattern) && match.position() == 0) {
                token = match.str();
                std::advance(it, match.length());
                currentPos += match.length();
            } else {
                token = *it;
                ++it;
                ++currentPos;
            }
        } else if (!std::isalnum(*it)) {  // Обрабатываем одиночные символы
            token = *it;
            ++it;
            ++currentPos;
        } else {  // Собираем слова и числа
            while (it != text.end() && std::isalnum(*it)) {
                token += *it;
                ++it;
                ++currentPos;
            }
        }

        callback(Token(token, startPos, index++, fileId));
    }
}

/**
 * @brief Разбивает текст на токены с применением фильтров.
 * @param text Входной текст.
 * @param fileId Идентификатор файла.
 * @param callback Функция обратного вызова для обработки отфильтрованных токенов.
 */
void Tokenizer::tokenizeFiltered(const std::string &text, FileId fileId, std::function<void(Token)> callback) {
    tokenizeRaw(text, fileId, [this, &callback](Token token) {
        std::string filteredToken = applyFilters(token.getBody());
        if (!filteredToken.empty()) {
            callback(Token(filteredToken, token.getPos(), token.getIndex(), token.getFileId()));
        }
    });
}

/**
 * @brief Применяет фильтры к токену.
 * @param token Исходный токен.
 * @return Отфильтрованный токен или пустая строка.
 *
 */
std::string Tokenizer::applyFilters(const std::string &token) {
    std::string *result = const_cast<std::string*>(&token);

    for (const auto &filter : filters) {
        *result = filter->process(*result);
        if (result->empty()) {
            return "";
        }
    }
    return *result;
}

/**
 * @brief Добавляет фильтр в список применяемых фильтров.
 * @param filter Указатель на объект фильтра.
 */
void Tokenizer::addFilter(Base* filter) {
    filters.push_back(filter);
}
