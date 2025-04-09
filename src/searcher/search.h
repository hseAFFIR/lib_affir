#pragma once

#include "../indexer/indexer.h"
#include <vector>
#include <string>

/**
 * @brief Class for performing index search operations
 */
class Search {
public:
    /**
     * @brief Structure for storing search results
     */
    struct SearchResult {
        std::string query;       ///< Original query
        PosMap posMap;           ///< Position map in files
        bool isPhrase;          ///< Whether the result is a phrase search
    };

    /**
     * @brief Constructor
     * @param indexer Reference to indexer
     */
    Search(Indexer& indexer);

    /**
     * @brief Main search method
     * @param query Query text (can be a single word or phrase)
     * @return Vector of search results
     * @throws std::invalid_argument if query is empty or too long
     */
    std::vector<SearchResult> search(const std::string& query) const;

    /**
     * @brief Prints search results to console
     * @param results Vector of search results
     */
    void printSearchResults(const std::vector<SearchResult>& results) const;

private:
    Indexer& indexer;                        ///< Reference to indexer

    /**
     * @brief Checks if words are adjacent in correct order
     * @param firstInfos Vector of token info for first word
     * @param secondInfos Vector of token info for second word
     * @return true if words are adjacent in correct order
     */
    bool areWordsAdjacent(const std::vector<TokenInfo>& firstInfos,
                         const std::vector<TokenInfo>& secondInfos) const;

    /**
     * @brief Searches for a single word
     * @param word Word to search for
     * @return Vector of search results
     */
    std::vector<SearchResult> searchSingleWord(const std::string& word) const;

    /**
     * @brief Searches for a phrase
     * @param words Vector of phrase words
     * @return Vector of search results
     */
    std::vector<SearchResult> searchPhrase(const std::vector<std::string>& words) const;

    /**
     * @brief Combines words into a phrase string
     * @param words Vector of words
     * @return Combined phrase string
     */
    static std::string combinePhrase(const std::vector<std::string>& words);

    /**
     * @brief Validates search query
     * @param query Query to validate
     * @throws std::invalid_argument if query is invalid
     */
    static void validateQuery(const std::string& query);
};