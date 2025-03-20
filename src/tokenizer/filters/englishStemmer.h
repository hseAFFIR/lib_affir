#ifndef ENGLISHSTEMMER_H
#define ENGLISHSTEMMER_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>
#include "base.h"

class EnglishStemmer : public Base {
public:
    /// Выполняет стемминг английского слова по алгоритму Porter2.
    /// :param word: слово в нижнем регистре без лишних символов
    /// :return: обрезанное слово
    std::string process(const std::string &word) const override;

    EnglishStemmer();

    FilterOrder getOrder() const override {
        return FilterOrder::EnglishStemmer;
    }

private:
    // Константы, определённые в конструкторе
    std::string english_vowels;
    std::vector<std::string> english_doubles;
    std::string english_valid_li_endings;
    const std::unordered_map<const std::string, const std::string> english_exceptions1;
    std::unordered_set<std::string> english_exceptions2;

    // Вспомогательные методы
    std::pair<std::string, std::string> english_set_regions(const std::string &word) const;
    std::string english_replace_suffixes(const std::string &word) const;
    bool english_is_short(const std::string &word, const std::string &R1) const;
    bool english_is_short_syllable(const std::string &word) const;
    std::string step2(const std::string &word, const std::string &R1) const;
    std::string step3(const std::string &word, const std::string &R1, const std::string &R2) const;
    std::string step4(const std::string &word, const std::string &R2) const;
    std::string step5(const std::string &word, const std::string &R1, const std::string &R2) const;

    // Вспомогательные inline-функции
    static bool ends_with(const std::string &s, const std::string &suffix);
    static bool contains_any(const std::string &s, const std::string &chars);
    static std::string to_lower(const std::string &s);
};

#endif // ENGLISHSTEMMER_H
