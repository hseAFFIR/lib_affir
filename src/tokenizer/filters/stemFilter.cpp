#include "stemFilter.h"
#include "../../logger/logger.h"

StemFilter::StemFilter() {
    logger = Logger::GetRootLogger();
    LOG_INFO(logger,"StemFilter module initialized");
}

bool CyrillicChar(const std::string &text, size_t index) {
    if (index + 1 >= text.size()) return false;

    unsigned char first = text[index];
    unsigned char second = text[index + 1];

    if (first == 0xD0) {
        return (second >= 0x90 && second <= 0xBF) || second == 0x81;
    } else if (first == 0xD1) {
        return (second >= 0x80 && second <= 0x8F) || second == 0x91;
    }

    return false;
}

std::string StemFilter::detect_language(const std::string &token) const {
    bool has_cyrillic = false;
    bool has_latin = false;

    for (size_t i = 0; i < token.size(); ++i) {
        unsigned char c = token[i];
        if (CyrillicChar(token, i)) {
            has_cyrillic = true;
            ++i;
        }
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            has_latin = true;
        }
    }

    if (has_cyrillic && !has_latin) {
        LOG_DEBUG(logger, "Russian language detected for word {}",token);
        return "ru";
    }
    if (has_latin && !has_cyrillic) {
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
