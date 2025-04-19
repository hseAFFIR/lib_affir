//
// Created by okosh on 14.03.2025.
//

#ifndef RUSSIAN_PORTER_STEMMER_H
#define RUSSIAN_PORTER_STEMMER_H

#include "base.h"
#include <string>
#include <vector>

/**
 * @class RussianPorterStemmer
 * @brief Класс для стемминга русских слов с использованием алгоритма Портера.
 *
 * Этот класс реализует стемминг русского языка, удаляя суффиксы и окончания для приведения слов к их основам.
 */
class RussianPorterStemmer:public Base{
private:
    std::tuple<std::string, std::string, std::string> russianSetRegions(std:: string &token);
    std::string step_1(std::string rv,std::string token);
    std:: string step_1_if(std::string token);
    std::string vowels; ///< Гласные буквы, используемые для разделения слова на части.
    std::vector<std::string> perfective_gerund_1; ///< Окончания совершенных деепричастий (группа 1).
    std::vector<std::string> perfective_gerund_2;
    std::vector<std::string> perfective_gerund;
    std::vector<std::string> participle;///< Окончания совершенных деепричастий (группа 2).
    std::vector<std::string> adjective; ///< Окончания прилагательных.
    std::vector<std::string> participle_1; ///< Окончания причастий (группа 1).
    std::vector<std::string> participle_2; ///< Окончания причастий (группа 2).
    std::vector<std::string> reflexive; ///< Окончания возвратных глаголов.
    std::vector<std::string> verb_1; ///< Окончания глаголов (группа 1).
    std::vector<std::string> verb_2;
    std::vector<std::string> verb;///< Окончания глаголов (группа 2).
    std::vector<std::string> noun; ///< Окончания существительных.
    std::vector<std::string> superlative; ///< Окончания превосходной степени прилагательных.
    std::vector<std::string> derivational; ///< Производные суффиксы.
public:
    RussianPorterStemmer();
    void process(std::string &token) override;
    FilterOrder getOrder() const override {
        return FilterOrder::RussianPorterStemmer;
    }

};

#endif // RUSSIAN_PORTER_STEMMER_H
