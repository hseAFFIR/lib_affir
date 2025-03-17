//
// Created by okosh on 14.03.2025.
//

#ifndef ENGLISH_STEMMER_H
#define ENGLISH_STEMMER_H

#include "base.h"
#include <string>

/**
 * @class EnglishStemmer
 * @brief Класс для стемминга английских слов.
 *
 * Наследуется от класса Base и реализует метод process для приведения слова к его корневой форме (стемминга).
 */
class EnglishStemmer : public Base {
public:
    /**
     * @brief Выполняет стемминг переданного слова.
     *
     * Метод принимает слово и возвращает его стемму (основу).
     * @param token Входное слово.
     * @return Стеммированное слово.
     */
    std::string process(const std::string &token) const override;
};

#endif // ENGLISH_STEMMER_H
