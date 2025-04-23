//
// Created by okosh on 14.03.2025.
//

#include "stemFilter.h"
#include "../../logger/logger.h"
#include <regex>


StemFilter::StemFilter() {
    logger = Logger::logger;
    LOG_INFO(logger,"StemFilter module initialized");
}

std::string StemFilter::detect_language(const std::string &token) const {
    if (std::regex_search(token, std::regex("[а-яА-Я]"))) {
        LOG_DEBUG(logger, "Russian language detected for word {}",token);
        return "ru";
    }
    if (std::regex_search(token, std::regex("[a-zA-Z]"))) {
        LOG_DEBUG(logger,"English language detected for word {}",token);
        return "en";
    }
    LOG_WARNING(logger,"Unknown language! {}",token);
    return "";
}

void StemFilter::process(std::string &token) {
    try {
        std::string lang = detect_language(token);
        if (lang == "ru") {
            russian_stemmer.process(token);
        }
        if (lang == "en") {
            english_stemmer.process(token);
        }
    } catch (...) {
        LOG_INFO(logger,"stemFilter::process - Undefined exception!");
    }
}
