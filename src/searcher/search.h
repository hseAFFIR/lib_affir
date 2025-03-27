//
// Created by amenk on 22.03.2025.
//

#ifndef LIB_AFFIR_SEARCH_H
#define LIB_AFFIR_SEARCH_H

#include <string>
#include <vector>
#include <utility> // for std::pair
#include "../indexer/indexer.h"
#include "../tokenizer/filters/stemFilter.h"

/**
 * @class Search
 * @brief The Search class provides functionality to search for tokens in indexed data.
 *
 * This class uses an Indexer to retrieve token information and a StemFilter to process tokens
 * before searching. It returns a list of file IDs and positions where the tokens are found.
 */
class Search {
public:
    /**
     * @brief Constructs a Search object.
     *
     * @param indexer Reference to an Indexer instance for token lookup.
     * @param stemFilter Reference to a StemFilter instance for token processing.
     */
    Search(Indexer& indexer, StemFilter& stemFilter);

    /**
     * @brief Searches for the given text in the indexed data.
     *
     * This method tokenizes the input text, processes each token using the StemFilter,
     * and retrieves their positions from the Indexer.
     *
     * @param text The text to search for.
     * @return A vector of pairs containing file IDs and positions where the tokens are found.
     */
    std::vector<std::pair<unsigned long, unsigned long>> find(const std::string& text);

private:
    Indexer& indexer; ///< Reference to the Indexer for token lookup.
    StemFilter& stemFilter; ///< Reference to the StemFilter for token processing.
};

#endif //LIB_AFFIR_SEARCH_H