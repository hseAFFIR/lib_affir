#include "search.h"
#include "../logger/logger.h"
#include "../tokenizer/tokenizer.h"
#include <iostream>


Searcher::Searcher(const MultiFileIndexStorage& storage, Indexer& indexer) 
    : storage(storage), indexer(indexer) {
}

std::vector<Searcher::SearchResult> Searcher::searchSingleWord(const std::string& word) const {
    std::vector<SearchResult> results;
    
    BigToken token = indexer.getTokenInfo(word);
    PosMap positions = token.getFilePositions();

    Logger::debug("Searcher","appearances: {} for word {}",positions.size(),word);
    
    if (!positions.empty()) {
        results.push_back({
            word,
            positions,
            false  // isPhrase = false for single word
        });
    }
    
    return results;
}

bool Searcher::areWordsAdjacent(FileId fileId, 
                              const std::vector<TokenInfo>& firstInfos,
                              const std::vector<TokenInfo>& secondInfos) const {
    // Check if both words have positions
    if (firstInfos.empty() || secondInfos.empty()) {
        return false;
    }
    
    // Get the last position of the first word
    const auto& lastFirstPos = firstInfos.back();
    // Get the last position of the second word
    const auto& lastSecondPos = secondInfos.back();
    
    // Check if the second word follows immediately after the first
    return lastSecondPos.wordPos - lastFirstPos.wordPos == 1;
}

std::vector<Searcher::SearchResult> Searcher::searchPhrase(const std::vector<std::string>& words) const {
    std::vector<SearchResult> results;
    
    if (words.empty()) {
        return results;
    }
    
    // Combine the entire phrase into one string
    std::string fullPhrase;
    for (size_t i = 0; i < words.size(); ++i) {
        if (i > 0) fullPhrase += " ";
        fullPhrase += words[i];
    }
    
    // Search for the first word
    auto firstWordResults = searchSingleWord(words[0]);
    
    // For each found first word
    for (const auto& firstWord : firstWordResults) {
        // Create a new position map only for found phrases
        PosMap phrasePositions;
        
        // Check each position of the first word
        for (const auto& [fileId, firstInfos] : firstWord.posMap) {
            bool foundPhrase = true;
            
            // Check the remaining words
            for (size_t i = 1; i < words.size(); ++i) {
                auto nextWordResults = searchSingleWord(words[i]);
                bool foundNext = false;
                
                // Check if the next word exists in the same file
                auto it = nextWordResults[0].posMap.find(fileId);
                if (it == nextWordResults[0].posMap.end()) {
                    foundPhrase = false;
                    break;
                }
                
                // Check if words follow each other
                if (areWordsAdjacent(fileId, firstInfos, it->second)) {
                    foundNext = true;
                }
                
                if (!foundNext) {
                    foundPhrase = false;
                    break;
                }
            }
            
            // If phrase is found, add only this position
            if (foundPhrase) {
                phrasePositions[fileId] = firstInfos;
            }
        }
        
        // If at least one complete phrase is found
        if (!phrasePositions.empty()) {
            results.push_back({
                fullPhrase,  // Use full phrase instead of first word
                phrasePositions,
                true  // isPhrase = true for phrase
            });
        }
    }
    
    return results;
}

std::vector<Searcher::SearchResult> Searcher::search(const std::string& query) const {
    Logger::info("Searcher", "Searching for: {}", query);
    if (query.empty()) {
        Logger::warn("Searcher", "Empty query!");
        return {};
    }    

    // Create tokenizer
    Tokenizer tokenizer({});
    FileId fileId = 1;
    
    // Vector to store tokens
    std::vector<Token> tokens;
    
    // Tokenize query and save results to vector
    tokenizer.tokenizeRaw(query, fileId, [&tokens](Token token) {
        tokens.push_back(token);
        Logger::debug("Searcher","pushed token: {}",token.getBody());
    });



    // If no tokens - return empty vector
    if (tokens.size() == 0) {
        Logger::warn("Searcher", "Empty vector after tokenization!");
      return {};
    }

    // If we have only one token - search as single word
    if (tokens.size() == 1) {
        Logger::info("Searcher", "Searching for single word: {}", tokens[0].getBody());
        return searchSingleWord(tokens[0].getBody());
    }
    
    // If multiple tokens - search as phrase
    std::vector<std::string> words;
    for (const auto& token : tokens) {
        words.push_back(token.getBody());
    }
    return searchPhrase(words);
}

void Searcher::printSearchResults(const std::vector<SearchResult>& results) const {
    for (const auto& result : results) {
        std::cout << "--------------------------------" << std::endl;
        Logger::info("Searcher", "Results found.");
        std::cout << "Results for query "<< result.query << std::endl;
        for (const auto& [fileId, tokenInfos] : result.posMap) {
            std::cout << "File ID: " << fileId << std::endl;
            std::cout << "  Positions: ";
            for (const auto& info : tokenInfos) {
                std::cout << "(pos=" << info.pos << ", wordPos=" << info.wordPos << ") ";
            }
            std::cout << std::endl;
        }
    }

    if (results.empty()) {
        std::cout << "No results found!" << std::endl;
    }
}