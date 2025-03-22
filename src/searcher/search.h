//
// Created by amenk on 22.03.2025.
//

#ifndef LIB_AFFIR_SEARCH_H
#define LIB_AFFIR_SEARCH_H


#include <string>
#include <vector>
#include <unordered_map>
#include "../models/bigToken.h"
#include "../indexer/indexer.h"

class Search {
public:
    Search(Indexer& indexer); // Конструктор, принимающий ссылку на Indexer
    std::vector<std::pair<unsigned long, unsigned long>> find(const std::string& text); // Основной метод поиска

private:
    Indexer& indexer; // Ссылка на Indexer для доступа к данным
    std::vector<std::string> tokenize(const std::string& text); // Вспомогательный метод для токенизации текста
};


#endif //LIB_AFFIR_SEARCH_H
