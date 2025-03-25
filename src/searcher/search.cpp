//
// Created by amenk on 22.03.2025.
//

#include "search.h"
#include <iostream>
#include "../logger/logger.h"
#include <cctype> // для ispunct
#include <algorithm> // для remove_if

std::string removePunctuation(const std::string& text) {
    std::string result = text;
    // Удаляем знаки препинания
    result.erase(std::remove_if(result.begin(), result.end(), ::ispunct), result.end());
    return result;
}

Search::Search(Indexer& indexer, StemFilter& stemFilter)
        : indexer(indexer), stemFilter(stemFilter) {
    Logger::warn("Search", "Search module initialized");
}
std::vector<std::pair<unsigned long, unsigned long>> Search::find(const std::string& text) {
    Logger::debug("Search", "Starting search for text: {}", text);
    std::vector<std::pair<unsigned long, unsigned long>> results;

    // Удаляем знаки препинания из текста
    std::string cleanedText = removePunctuation(text);

    // Разбиваем текст на токены
    std::vector<std::string> tokens;
    std::istringstream stream(cleanedText);
    std::string token;
    while (stream >> token) {
        tokens.push_back(token);
    }
    if (text.empty()){
        Logger::error("Search", "Empty search text provided!");
    }

    // Обрабатываем каждый токен с помощью StemFilter
    for (const std::string& token : tokens) {
        std::string stemmedToken = stemFilter.process(token);

        // Ищем обработанный токен в индексе
        const BigToken& bigToken = indexer.getTokenInfo(stemmedToken);

        // Получаем информацию о позициях токена в файлах
        const auto& filePositions = bigToken.getFilePositions();

        // Добавляем все найденные позиции в результаты
        for (const auto& [fileId, positions] : filePositions) {
            for (const TokenInfo& info : positions) {
                results.emplace_back(fileId, info.pos);
            }
        }
    }
    Logger::info("Search", "Search completed successfully");
    return results;
}