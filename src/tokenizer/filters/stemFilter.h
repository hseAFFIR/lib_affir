//
// Created by okosh on 14.03.2025.
//

#ifndef STEM_FILTER_H
#define STEM_FILTER_H

#include <quill/Logger.h>
#include "base.h"
#include "russianPorterStemmer.h"
#include "englishStemmer.h"

/**
 * @class StemFilter
 * @brief Класс для стемминга слов на разных языках.
 *
 * Этот класс определяет язык слова и применяет соответствующий алгоритм стемминга:
 * - RussianPorterStemmer для русского языка.
 * - EnglishStemmer для английского языка.
 */
class StemFilter : public Base {
private:
    quill::Logger* logger;
    RussianPorterStemmer russianStemmer; ///< Стеммер для русского языка.
    EnglishStemmer englishStemmer; ///< Стеммер для английского языка.

    /**
     * @brief Определяет язык переданного слова.
     *
     * Используется для выбора соответствующего стеммера.
     * @param token Входное слово.
     * @return Строка, указывающая язык ("ru" для русского, "en" для английского).
     */
    std::string detectLanguage(const std::string &token) const;

public:
    /**
     * @brief Конструктор по умолчанию.
     *
     * Инициализирует стеммеры для обработки слов на разных языках.
     */
    StemFilter();

    /**
     * @brief Выполняет стемминг переданного слова.
     *
     * Определяет язык слова и применяет соответствующий стеммер.
     * @param token Входное слово.
     * @return Стеммированное слово.
     */
    void process(std::string &token) override;

    FilterOrder getOrder() const override {
        return FilterOrder::StemFilter;
    }
};

#endif // STEM_FILTER_H
