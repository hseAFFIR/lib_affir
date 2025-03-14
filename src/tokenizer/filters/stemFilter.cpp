//
// Created by okosh on 14.03.2025.
//

#include "stemFilter.h"
#include <regex>

StemFilter::StemFilter() { }

std::string StemFilter::detect_language(const std::string &token) const {
    if (std::regex_search(token, std::regex("[а-яА-Я]"))) {
        return "ru";
    }
    if (std::regex_search(token, std::regex("[a-zA-Z]"))) {
        return "en";
    }
    return "";
}

std::string StemFilter::process(const std::string &token) const {
    try {
        std::string lang = detect_language(token);
        if (lang == "ru") {
            return russian_stemmer.process(token);
        }
        if (lang == "en") {
            return english_stemmer.process(token);
        }
        return token;
    } catch (...) {
        return token;
    }
}
