#include "Tokenizer.h"
#include <cctype>
#include <iostream>

Tokenizer::Tokenizer(std::vector<Base*> filters)
    : filters(std::move(filters)), htmlPattern(R"(<\/?\w+.*?>)") {}

void Tokenizer::tokenizeRaw(const std::string &text, FileId fileId, std::function<void(Token)> callback) {
    size_t currentPos = 0;
    size_t index = 0;

    auto it = text.begin();
    while (it != text.end()) {
        while (it != text.end() && std::isspace(*it)) {
            ++it;
            ++currentPos;
        }

        if (it == text.end()) break;

        size_t startPos = currentPos;
        std::string token;

        if (*it == '<') {
            std::smatch match;
            std::string remainingText(it, text.end());

            if (std::regex_search(remainingText, match, htmlPattern) && match.position() == 0) {
                token = match.str();
                std::advance(it, match.length());
                currentPos += match.length();
            } else {
                token = *it;
                ++it;
                ++currentPos;
            }
        } else if (!std::isalnum(*it)) {
            token = *it;
            ++it;
            ++currentPos;
        } else {
            while (it != text.end() && std::isalnum(*it)) {
                token += *it;
                ++it;
                ++currentPos;
            }
        }

        callback(Token(token, startPos, index++, fileId));
    }
}

void Tokenizer::tokenizeFiltered(const std::string &text, FileId fileId, std::function<void(Token)> callback) {
    tokenizeRaw(text, fileId, [this, &callback](Token token) {
        std::string filteredToken = applyFilters(token.getBody());
        if (!filteredToken.empty()) {
            callback(Token(filteredToken, token.getPos(), token.getIndex(), token.getFileId()));
        }
    });
}

std::string Tokenizer::applyFilters(const std::string &token) {
    std::string result = token;
    for (const auto &filter : filters) {
        result = filter->process(result);
        if (result.empty()) {
            return "";
        }
    }
    return result;
}

void Tokenizer::addFilter(Base* filter) {
    filters.push_back(filter);
}