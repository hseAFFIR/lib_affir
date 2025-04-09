#pragma once

#include "../indexer/indexer.h"
#include "../storages/indexes/multi/multiFileIndexStorage.h"
#include <vector>
#include <string>

/**
 * @brief Class for performing index search
 */
class Searcher {
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
     * @param storage Reference to storage
     * @param indexer Reference to indexer
     */
    Searcher(const MultiFileIndexStorage& storage, Indexer& indexer);

    /**
     * @brief Main search method
     * @param query Query text (can be a single word or phrase)
     * @return Vector of search results
     */
    std::vector<SearchResult> search(const std::string& query) const;

    /**
     * @brief Prints search results to console
     * @param results Vector of search results
     */
    void printSearchResults(const std::vector<SearchResult>& results) const;

private:
    const MultiFileIndexStorage& storage;    /// Reference to storage
    Indexer& indexer;                        /// Reference to indexer

    /**
     * @brief Checks if words are adjacent in correct order
     * @param firstPos Position of first word
     * @param secondPos Position of second word
     * @return true if words are adjacent in correct order
     */
    bool areWordsAdjacent(FileId fileId, 
                         const std::vector<TokenInfo>& firstInfos,
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
};