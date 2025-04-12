#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <regex>
#include <functional>
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

    /**
     * @brief Разбивает текст на токены без применения фильтров.
     * @param text Входной текст для токенизации.
     * @param fileId Идентификатор файла, откуда взят текст.
     * @param callback Функция обратного вызова, которая будет вызвана для каждого токена.
     *
     * Этот метод проходит по строке, идентифицирует токены (слова, символы, HTML-теги) и
     * вызывает callback для каждого токена с информацией о его позиции и типе.
     */
    void tokenizeRaw(const std::string &text, std::function<void(Token)> callback, std::optional<FileId> fileId = std::nullopt);

    /**
     * @brief Разбивает текст на токены с применением фильтров.
     * @param text Входной текст для токенизации.
     * @param fileId Идентификатор файла.
     * @param callback Функция обратного вызова, которая будет вызвана для каждого отфильтрованного токена.
     *
     * Этот метод вызывает `tokenizeRaw`, а затем применяет фильтры к каждому токену, передавая
     * отфильтрованные токены в callback.
     */
    void tokenize(const std::string &text, std::function<void(Token)> callback, std::optional<FileId> fileId = std::nullopt);

private:
    std::vector<Base*> filters; /**< Список фильтров, применяемых к токенам. */
    const std::regex htmlPattern;     /**< Регулярное выражение для поиска HTML-тегов. */
    const size_t htmlPatternLimit;

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
