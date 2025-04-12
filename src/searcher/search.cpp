#include "search.h"
#include "../logger/logger.h"
#include "../tokenizer/tokenizer.h"
#include <iostream>
#include <stdexcept>

void Search::validateQuery(const std::string& query) {
    if (query.empty())
        throw std::invalid_argument("Query cannot be empty");
    if (query.length() > MAX_QUERY_LENGTH)
        throw std::invalid_argument("Query is too long");
}

std::string Search::combinePhrase(const std::vector<Token>& tokens) {
    std::string result;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (i > 0) result += " ";
        result += tokens[i].body;
    }
    return result;
}

Search::Search(const std::vector<Base*> &filters, IIndexStorage &indStor) {
    indexer = new Indexer(indStor);
    tokenizer = new Tokenizer(filters);
}

PosMap Search::getPhrasePositions(const std::vector<Token>& tokens) const {
    BigToken firstWordResults = indexer->getTokenInfo(tokens[0].body);
    auto &firstWordPoses = const_cast<PosMap &>(firstWordResults.getFilePositions());

    if (firstWordPoses.empty())
        return {};

    PosMap phrasePositions;

    for (const auto& [fileId, firstInfos] : firstWordPoses) {
        for (const TokenInfo& firstTokenInfo : firstInfos) {
            std::vector<TokenInfo> currentPhrasePositions = { firstTokenInfo };
            bool foundPhrase = true;

            for (size_t i = 1; i < tokens.size(); ++i) {
                BigToken nextWordResults = indexer->getTokenInfo(tokens[i].body);
                auto &nextWordPoses = const_cast<PosMap &>(nextWordResults.getFilePositions());

                if (nextWordPoses.empty()) {
                    foundPhrase = false;
                    break;
                }

                auto it = nextWordPoses.find(fileId);
                // Следующее слово есть в индексе, но не в том же файле
                if (it == nextWordPoses.end()) {
                    foundPhrase = false;
                    break;
                }

                // Find the next word's position that is adjacent to the last position in current phrase
                bool foundNext = false;
                for (const TokenInfo& nextPos : it->second) {
                    if (nextPos.wordPos - currentPhrasePositions.back().wordPos == 1) {
                        currentPhrasePositions.push_back(nextPos);
                        foundNext = true;
                        break;
                    }
                }

                if (!foundNext) {
                    foundPhrase = false;
                    break;
                }
            }

            if (foundPhrase)
                // Only add the first position of the complete phrase
                phrasePositions[fileId].push_back(currentPhrasePositions[0]);
        }
    }
    
    return phrasePositions;
}

Search::SearchResult Search::search(const std::string& query) const {
    validateQuery(query);
    Logger::info("Search", "Searching for: {}", query);

    std::vector<Token> tokens;
    std::string text = query;
    tokenizer->tokenize(text);
    while (tokenizer->hasNext()) {
        Token token = tokenizer->next();
        tokens.push_back(token);
        Logger::debug("Search", "pushed token: {}", token.body);
    }

    if (tokens.empty()) {
        Logger::warn("Search", "Empty vector after tokenization!");
        return {};
    }

    return SearchResult(combinePhrase(tokens), getPhrasePositions(tokens));
}

void Search::printSearchResults(const std::vector<SearchResult>& results) const {
    if (results.empty()) {
        Logger::warn("Search", "No results found!");
        return;
    }

    for (const auto& result : results) {
        std::cout << "--------------------------------" << std::endl;
        Logger::info("Search", "Found results for query {}:", result.query);
        for (const auto& [fileId, tokenInfos] : result.posMap) {
            std::cout << "File ID: " << fileId << std::endl;
            std::cout << "  Positions: ";
            for (const auto& info : tokenInfos)
                std::cout << "(pos=" << info.pos << ", wordPos=" << info.wordPos << ") ";
            std::cout << std::endl;
        }
    }
}

Search::~Search() {
    delete tokenizer;
    delete indexer;
}