#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <optional>
#include <vector>
#include <regex>
#include <functional>
#include <optional>
#include "filters/base.h"
#include "../common.h"
#include <optional>

/**
 * @class Tokenizer
 * @brief Класс для токенизации текста с возможностью применения фильтров.
 *
 * Класс предоставляет функциональность для разбивки текста на токены, а также
 * применения различных фильтров к этим токенам. Он поддерживает работу как
 * с необработанными текстами, так и с текстами, уже прошедшими фильтрацию.
 */
class Tokenizer {
public:
    /**
     * @brief Конструктор класса tokenizer.
     * @param filters Вектор указателей на объекты фильтров, которые будут применяться к токенам.
     *
     * Этот конструктор инициализирует объект с набором фильтров и шаблоном для поиска HTML-тегов.
     */
    explicit Tokenizer(std::vector<Base*> filters)
        : filters(std::move(filters)),
            htmlPattern(R"(<\/?\w+.*?>)"),
            htmlPatternLimit(10) { };

    explicit Tokenizer() : Tokenizer({}) { }

    bool hasNext();

    Token next();

    /**
     * @brief Разбивает текст на токены с применением фильтров.
     * @param text Входной текст для токенизации.
     * @param fileId Идентификатор файла.
     * @param callback Функция обратного вызова, которая будет вызвана для каждого отфильтрованного токена.
     */
    void tokenize(std::string &text, FileId inFileId = 0);

private:
    std::vector<Base*> filters; /**< Список фильтров, применяемых к токенам. */
    const std::regex htmlPattern;     /**< Регулярное выражение для поиска HTML-тегов. */
    const size_t htmlPatternLimit;

    bool prepareNext();
    Token preparedToken;

    size_t currentPos = 0;
    size_t wordPos = 0;
    size_t i = 0;
    std::string text;
    FileId fileId;

    /**
     * @brief Применяет все фильтры к токену.
     * @param token Исходный токен.
     * @return Отфильтрованный токен или пустая строка.
     *
     * Этот метод применяет все фильтры в списке к токену и возвращает результат.
     * Если после применения фильтров токен становится пустым, возвращается пустая строка.
     */
    void applyFilters(std::string &token);
};

#endif // TOKENIZER_H
