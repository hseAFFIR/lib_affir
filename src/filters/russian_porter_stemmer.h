//
// Created by okosh on 14.03.2025.
//

#ifndef RUSSIAN_PORTER_STEMMER_H
#define RUSSIAN_PORTER_STEMMER_H
#include "base.h"
#include <string>
#include <vector>
class RussianPorterStemmer : public base {
private:
    std::string vowels;

    // Списки окончаний
    std::vector<std::string> perfective_gerund_1;
    std::vector<std::string> perfective_gerund_2;
    std::vector<std::string> adjective;
    std::vector<std::string> participle_1;
    std::vector<std::string> participle_2;
    std::vector<std::string> reflexive;
    std::vector<std::string> verb_1;
    std::vector<std::string> verb_2;
    std::vector<std::string> noun;
    std::vector<std::string> superlative;
    std::vector<std::string> derivational;

    // Вспомогательные методы
    void initialize_endings();
    std::tuple<std::string, std::string, std::string> russian_set_regions(const std::string& word) const;
    std::string step_1(const std::string& rv, const std::string& word) const;
    std::string step_1_if(const std::string& word, const std::string& rv) const;

public:
    RussianPorterStemmer();

    // Реализация метода process
    std::string process(const std::string& token) const override;
};



#endif //RUSSIAN_PORTER_STEMMER_H
