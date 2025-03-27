//
// Created by okosh on 14.03.2025.
//

#include "stemFilter.h"
#include <regex>
#include "../../logger/logger.h"

StemFilter::StemFilter() {
    Logger::info("StemFilter", "StemFilter module initialized");
}

std::string StemFilter::detect_language(const std::string &token) const {
    if (std::regex_search(token, std::regex("[а-яА-Я]"))) {
        Logger::debug("StemFilter", "Russian language detected for word {}",token);
        return "ru";
    }
    if (std::regex_search(token, std::regex("[a-zA-Z]"))) {
        Logger::debug("StemFilter", "English language detected for word {}",token);
        return "en";
    }
    Logger::warn("StemFilter", "Unknown language! {}",token);
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
        Logger::error("stemFilter::process","Undefined exception!");
        return token;
    }
}
