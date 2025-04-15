#include "storages/indexes/multi/multiFileIndexStorage.h"
#include "indexer/indexer.h"
#include "searcher/search.h"
#include "logger/logger.h"
#include "processing/dataHandler.h"
#include "tokenizer/filters/filters.h"

#include <cassert>
#include <fstream>
#include "tokenizer/filters/russianPorterStemmer.h"

int main() {
    system("chcp 65001");
    Logger::init("logs/log.txt");
    Logger::info("Main", "Application started");

    std::vector<Base*> filters = {new StemFilter()};

    std::string text = "hello Ёлка маленькая";

    auto *tokenizer = new Tokenizer(filters);

    tokenizer->tokenize(text, 1);

    while(tokenizer->hasNext()) {
        Token token = tokenizer->next();
        std::cout << "Token: " << token.body << " | Pos: " << token.info.pos<< " | wordPos: "<< token.info.wordPos << std::endl;
    }

    delete tokenizer;

    for (auto f : filters) delete f;
    return 0;
}
