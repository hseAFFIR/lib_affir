#include "search.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include "../logger/logger.h"

Search::Search(Indexer& indexer, StemFilter& stemFilter)
        : indexer(indexer), stemFilter(stemFilter) {
    Logger::info("Search", "Initialized");
}

std::vector<std::string> Search::splitIntoWords(const std::string& phrase) const {
    std::string noQuotes = phrase.substr(1, phrase.size() - 2); // Удаляем кавычки
    std::istringstream iss(noQuotes);
    return {std::istream_iterator<std::string>{iss},
            std::istream_iterator<std::string>{}};
}


std::vector<std::string> Search::processTokens(const std::vector<std::string>& tokens) const {
    std::vector<std::string> stemmedTokens;
    for (const auto& token : tokens) {
        stemmedTokens.push_back(stemFilter.process(token));
        Logger::debug("Search", "Стемминг: '{}' → '{}'", token, stemmedTokens.back());
    }
    return stemmedTokens;
}

bool Search::isPhrase(const std::string& text) const {
    return text.starts_with('"') && text.ends_with('"');
}

// search.cpp
std::unordered_map<unsigned long, std::vector<TokenInfo>> Search::find(const std::string& text) {
    std::unordered_map<unsigned long, std::vector<TokenInfo>> results;
    if (text.empty()) {
        Logger::error("Search", "Empty query");
        return results;
    }


    if (isPhrase(text)) { // Фраза в кавычках
        std::vector<std::string> words = splitIntoWords(text);
        std::vector<std::string> stemmedWords = processTokens(words); // Используем наш новый метод

        // Ищем все слова в индексе
        std::vector<BigToken> stemmedTokens;
        for (const auto& word : stemmedWords) {
            stemmedTokens.push_back(indexer.getTokenInfo(word));
        }

        // Проверяем, что все слова есть в одних файлах
        for (const auto& [fileId, positions] : stemmedTokens[0].getFilePositions()) {
            bool allFound = true;
            for (size_t i = 1; i < stemmedTokens.size(); ++i) {
                if (!stemmedTokens[i].getFilePositions().count(fileId)) {
                    allFound = false;
                    break;
                }
            }
            if (allFound) {
                results[fileId] = positions; // Добавляем позиции первого слова
            }
        }
    } else {
        // Обработка одиночного слова
        std::string stemmed = stemFilter.process(text);
        results = indexer.getTokenInfo(stemmed).getFilePositions();
    }

    return results;
}