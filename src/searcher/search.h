#ifndef LIB_AFFIR_SEARCH_H
#define LIB_AFFIR_SEARCH_H
#include "../indexer/indexer.h"
#include "../tokenizer/tokenizer.h"
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
    };

    /**
     * @brief Constructor
     * @param indexer Reference to indexer
     */
    explicit Search(const std::vector<Base*> &filters, IIndexStorage &indStor);
    explicit Search(IIndexStorage &indStor) : Search({}, indStor) { };

    virtual ~Search();

    /**
     * @brief Main search method
     * @param query Query text (can be a single word or phrase)
     * @return Vector of search results
     * @throws std::invalid_argument if query is empty or too long
     */
    SearchResult search(std::string& query) const;

    /**
     * @brief Prints search results to console
     * @param results Vector of search results
     */
    void printSearchResults(const std::vector<SearchResult>& results) const;
    void printSearchResults(const SearchResult& result) const { printSearchResults(std::vector<SearchResult>{result}); };

private:
    Tokenizer *tokenizer;
    Indexer *indexer;                        ///< Reference to indexer
    static constexpr size_t MAX_QUERY_LENGTH = 1000;

    /**
     * @brief Searches for a phrase
     * @param words Vector of phrase words
     * @return Vector of search results
     */
    PosMap getPhrasePositions(const std::vector<Token>& tokens) const;

    /**
     * @brief Combines words into a phrase string
     * @param words Vector of words
     * @return Combined phrase string
     */
    static std::string combinePhrase(const std::vector<Token>& tokens);

    /**
     * @brief Validates search query
     * @param query Query to validate
     * @throws std::invalid_argument if query is invalid
     */
    static void validateQuery(const std::string& query);
};
#endif // LIB_AFFIR_SEARCH_H