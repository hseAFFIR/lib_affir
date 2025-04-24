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

std::string StemFilter::detectLanguage(const std::string &token) const {
    bool hasCyrillic = false;
    bool hasLatin = false;

    for (size_t i = 0; i < token.size(); ++i) {
        unsigned char c = token[i];
        if (CyrillicChar(token, i)) {
            hasCyrillic = true;
            ++i;
        }
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            hasLatin = true;
        }
    }

    if (hasCyrillic && !hasLatin) {
        LOG_DEBUG(logger, "Russian language detected for word {}",token);
        return "ru";
    }
    if (hasLatin && !hasCyrillic) {
        LOG_DEBUG(logger,"English language detected for word {}",token);
        return "en";
    }
    LOG_WARNING(logger,"Unknown language! {}",token);
    return "";
}

void StemFilter::process(std::string &token) {
    try {
        std::string lang = detectLanguage(token);
        if (lang == "ru") {
            russianStemmer.process(token);
        }
        if (lang == "en") {
            englishStemmer.process(token);
        }
    } catch (...) {
        LOG_INFO(logger,"stemFilter::process - Undefined exception!");
    }
}
