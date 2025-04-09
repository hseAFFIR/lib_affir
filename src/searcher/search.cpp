#include "search.h"
#include "../logger/logger.h"
#include "../tokenizer/tokenizer.h"
#include <iostream>
#include <stdexcept>

constexpr size_t MAX_QUERY_LENGTH = 1000;

void Search::validateQuery(const std::string& query) {
    if (query.empty()) {
        throw std::invalid_argument("Query cannot be empty");
    }
    if (query.length() > MAX_QUERY_LENGTH) {
        throw std::invalid_argument("Query is too long");
    }
}

std::string Search::combinePhrase(const std::vector<std::string>& words) {
    std::string result;
    for (size_t i = 0; i < words.size(); ++i) {
        if (i > 0) result += " ";
        result += words[i];
    }
    return result;
}

Search::Search(Indexer& indexer) 
    : indexer(indexer) {
}

std::vector<Search::SearchResult> Search::searchSingleWord(const std::string& word) const {
    std::vector<SearchResult> results;
    
    BigToken token = indexer.getTokenInfo(word);
    PosMap positions = token.getFilePositions();

    Logger::debug("Search", "appearances: {} for word {}", positions.size(), word);
    
    if (!positions.empty()) {
        results.push_back({
            word,
            positions,
            false  // isPhrase = false for single word
        });
    }
    
    return results;
}

bool Search::areWordsAdjacent(const std::vector<TokenInfo>& firstInfos,
                            const std::vector<TokenInfo>& secondInfos) const {
    if (firstInfos.empty() || secondInfos.empty()) {
        return false;
    }
    
    const auto& lastFirstPos = firstInfos.back();
    const auto& lastSecondPos = secondInfos.back();
    
    return lastSecondPos.wordPos - lastFirstPos.wordPos == 1;
}

std::vector<Search::SearchResult> Search::searchPhrase(const std::vector<std::string>& words) const {
    if (words.empty()) {
        return {};
    }
    
    std::string fullPhrase = combinePhrase(words);
    auto firstWordResults = searchSingleWord(words[0]);
    
    if (firstWordResults.empty()) {
        return {};
    }
    
    std::vector<SearchResult> results;
    
    for (const auto& firstWord : firstWordResults) {
        PosMap phrasePositions;
        
        for (const auto& [fileId, firstInfos] : firstWord.posMap) {
            bool foundPhrase = true;
            
            for (size_t i = 1; i < words.size(); ++i) {
                auto nextWordResults = searchSingleWord(words[i]);
                if (nextWordResults.empty()) {
                    foundPhrase = false;
                    break;
                }
                
                auto it = nextWordResults[0].posMap.find(fileId);
                if (it == nextWordResults[0].posMap.end() || 
                    !areWordsAdjacent(firstInfos, it->second)) {
                    foundPhrase = false;
                    break;
                }
            }
            
            if (foundPhrase) {
                phrasePositions[fileId] = firstInfos;
            }
        }
        
        if (!phrasePositions.empty()) {
            results.push_back({
                fullPhrase,
                phrasePositions,
                true
            });
        }
    }
    
    return results;
}

std::vector<Search::SearchResult> Search::search(const std::string& query) const {
    validateQuery(query);
    Logger::info("Search", "Searching for: {}", query);

    Tokenizer tokenizer({});
    FileId fileId = 1;
    std::vector<Token> tokens;
    
    tokenizer.tokenizeRaw(query, fileId, [&tokens](Token token) {
        tokens.push_back(token);
        Logger::debug("Search", "pushed token: {}", token.getBody());
    });

    if (tokens.empty()) {
        Logger::warn("Search", "Empty vector after tokenization!");
        return {};
    }

    if (tokens.size() == 1) {
        Logger::info("Search", "Searching for single word: {}", tokens[0].getBody());
        return searchSingleWord(tokens[0].getBody());
    }
    
    std::vector<std::string> words;
    words.reserve(tokens.size());
    for (const auto& token : tokens) {
        words.push_back(token.getBody());
    }
    return searchPhrase(words);
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
            for (const auto& info : tokenInfos) {
                std::cout << "(pos=" << info.pos << ", wordPos=" << info.wordPos << ") ";
            }
            std::cout << std::endl;
        }
    }
}