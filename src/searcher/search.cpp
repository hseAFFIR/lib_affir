//
// Created by amenk on 22.03.2025.
//

#include "search.h"
#include <iostream>

Search::Search(Indexer& indexer) : indexer(indexer) {}

std::vector<std::string> Search::tokenize(const std::string& text) {
    // Простая токенизация по пробелам (можно заменить на более сложную логику)
    std::vector<std::string> tokens;
    std::string token;
    for (char ch : text) {
        if (ch == ' ') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += ch;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::pair<unsigned long, unsigned long>> Search::find(const std::string& text) {
    std::vector<std::pair<unsigned long, unsigned long>> results;

    // Токенизация входного текста
    std::vector<std::string> tokens = tokenize(text);

    // Поиск каждого токена в индексе
    for (const std::string& token : tokens) {
        const BigToken& bigToken = indexer.getTokenInfo(token);

        // Получаем информацию о позициях токена в файлах
        const auto& filePositions = bigToken.getFilePositions();

        // Добавляем все найденные позиции в результаты
        for (const auto& [fileId, positions] : filePositions) {
            for (const TokenInfo& info : positions) {
                results.emplace_back(fileId, info.pos);
            }
        }
    }

    return results;
}