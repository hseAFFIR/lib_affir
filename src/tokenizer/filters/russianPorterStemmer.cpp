//
// Created by okosh on 14.03.2025.
//

#include "russianPorterStemmer.h"
#include "../../logger/logger.h"
#include <algorithm>
#include <regex>

RussianPorterStemmer::RussianPorterStemmer() {
    vowels = "аеиоуыэюя";
    initialize_endings();
}

void RussianPorterStemmer::initialize_endings() {
    perfective_gerund_1 = {"в", "вши", "вшись"};
    perfective_gerund_2 = {"ив", "ивши", "ившись", "ыв", "ывши", "ывшись"};
    adjective = {"ее", "ие", "ые", "ое", "ими", "ыми", "ей", "ий", "ый", "ой", "ем", "им", "ым", "ом", "его", "ого",
                 "ему", "ому", "их", "ых", "ую", "юю", "ая", "яя", "ою", "ею"};
    participle_1 = {"ем", "нн", "вш", "ющ", "щ"};
    participle_2 = {"ивш", "ывш", "ующ"};
    reflexive = {"ся", "сь"};
    verb_1 = {"ла", "на", "ете", "йте", "ли", "й", "л", "ем", "н", "ло", "но", "ет", "ют", "ны", "ть", "ешь", "нно"};
    verb_2 = {"ила", "ыла", "ена", "ейте", "уйте", "ите", "или", "ыли", "ей", "уй", "ил", "ыл", "им", "ым", "ен", "ило",
              "ыло", "ено", "ят", "ует", "уют", "ит", "ыт", "ены", "ить", "ыть", "ишь", "ую", "ю"};
    noun = {"а", "ев", "ов", "ие", "ье", "е", "иями", "ями", "ами", "еи", "ии", "и", "ией", "ей", "ой", "ий", "й",
            "иям", "ям", "ием", "ем", "ам", "ом", "о", "у", "ах", "иях", "ях", "ы", "ь", "ию", "ью", "ю", "ия", "ья",
            "я"};
    superlative = {"ейш", "ейше"};
    derivational = {"ост", "ость"};
}

std::tuple<std::string, std::string, std::string>
RussianPorterStemmer::russian_set_regions(const std::string &word) const {
    std::string rv, r1, r2;
    for (size_t i = 0; i < word.size(); ++i) {
        if (vowels.find(word[i]) != std::string::npos) {
            rv = word.substr(i + 1);
            break;
        }
    }
    for (size_t i = 0; i < word.size() - 1; ++i) {
        if (vowels.find(word[i]) != std::string::npos && vowels.find(word[i + 1]) == std::string::npos) {
            r1 = word.substr(i + 2);
            break;
        }
    }
    for (size_t i = 0; i < r1.size() - 1; ++i) {
        if (vowels.find(r1[i]) != std::string::npos && vowels.find(r1[i + 1]) == std::string::npos) {
            r2 = r1.substr(i + 2);
            break;
        }
    }
    return {rv, r1, r2};
}

std::string RussianPorterStemmer::process(const std::string &token) const {
    std::string word = token;
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    std::replace(word.begin(), word.end(), 'ё', 'е');

    auto [rv, r1, r2] = russian_set_regions(word);

    word = step_1(rv, word);
    std::tie(rv, r1, r2) = russian_set_regions(word);

    if (rv.ends_with("и")) {
        word.pop_back();
        std::tie(rv, r1, r2) = russian_set_regions(word);
    }

    for (const auto &ending: derivational) {
        if (r2.ends_with(ending)) {
            word.erase(word.size() - ending.size());
            std::tie(rv, r1, r2) = russian_set_regions(word);
            break;
        }
    }

    for (const auto &ending: superlative) {
        if (rv.ends_with(ending)) {
            word.erase(word.size() - ending.size());
            std::tie(rv, r1, r2) = russian_set_regions(word);
            break;
        }
    }

    if (rv.ends_with("нн")) {
        word.pop_back();
    }
    else if (rv.ends_with("ь")) {
        word.pop_back();
    }

    Logger::debug("russianPorterStemmer", "{} -> {}",token,word);
    return word;
}

std::string RussianPorterStemmer::step_1(const std::string &rv, const std::string &word) const {
    bool ends_with_p_gerund = false;
    std::string result = word;

    for (const auto &ending: perfective_gerund_1) {
        if (rv.ends_with(ending)) {
            std::regex pattern("(а|я)" + ending + "$");
            if (std::regex_search(rv, pattern)) {
                result.erase(result.size() - ending.size());
                ends_with_p_gerund = true;
            }
        }
    }

    for (const auto &ending: perfective_gerund_2) {
        if (rv.ends_with(ending)) {
            result.erase(result.size() - ending.size());
            ends_with_p_gerund = true;
        }
    }

    if (!ends_with_p_gerund) {
        result = step_1_if(result, rv);
    }

    return result;
}

std::string RussianPorterStemmer::step_1_if(const std::string &word, const std::string &rv) const {
    std::string result = word;

    for (const auto &ending: reflexive) {
        if (rv.ends_with(ending)) {
            result.erase(result.size() - ending.size());
            break;
        }
    }

    for (const auto &ending: adjective) {
        if (rv.ends_with(ending)) {
            result.erase(result.size() - ending.size());

            for (const auto &participle_ending: participle_1) {
                if (rv.ends_with(participle_ending)) {
                    std::regex pattern("(а|я)" + participle_ending + "$");
                    if (std::regex_search(rv, pattern)) {
                        result.erase(result.size() - participle_ending.size());
                        return result;
                    }
                }
            }

            for (const auto &participle_ending: participle_2) {
                if (rv.ends_with(participle_ending)) {
                    result.erase(result.size() - participle_ending.size());
                    return result;
                }
            }

            return result;
        }
    }

    for (const auto &ending: verb_1) {
        if (rv.ends_with(ending)) {
            std::regex pattern("(а|я)" + ending + "$");
            if (std::regex_search(rv, pattern)) {
                result.erase(result.size() - ending.size());
                return result;
            }
        }
    }

    for (const auto &ending: verb_2) {
        if (rv.ends_with(ending)) {
            result.erase(result.size() - ending.size());
            return result;
        }
    }

    for (const auto &ending: noun) {
        if (rv.ends_with(ending)) {
            result.erase(result.size() - ending.size());
            return result;
        }
    }

    return result;
}