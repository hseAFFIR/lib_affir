#ifndef LIB_AFFIR_STOPWORDS_H
#define LIB_AFFIR_STOPWORDS_H

#include "base.h"
#include <string>
#include <unordered_set>


/**
 * @class StopWords
 * @brief A filter that removes stop words from tokens.
 *
 * This class processes input tokens and removes common stop words
 * in both Russian and English. The stop words are loaded from external files.
 */
class StopWords : public Base {
public:
    /**
     * @brief Processes a token and removes it if it's a stop word.
     * 
     * If the token is found in the stop words list (either Russian or English),
     * an empty string is returned. Otherwise, the original token is returned.
     *
     * @param token The input token to be processed.
     * @return The original token if it is not a stop word, otherwise an empty string.
     */
    std::string process(const std::string &token) const override;

    /**
     * @brief Returns the filter order associated with the stopWords.
     *
     * This method provides a fixed order for the filter to ensure proper sorting.
     *
     * @return FilterOrder The order of the stopWords filter.
     */
    FilterOrder getOrder() const override {
        return FilterOrder::StopWords;
    }
private:
    static std::unordered_set<std::string> stopWordsRu;  ///< Set of Russian stop words.
    static std::unordered_set<std::string> stopWordsEn;  ///< Set of English stop words.
    static bool initialized;  ///< Flag indicating whether stop words have been initialized.
    /**
     * @brief Loads stop words from external files.
     *
     * Reads stop word lists from `stop_words_ru.txt` and `stop_words_en.txt`
     * and populates the corresponding sets. This method is only executed once.
     */
    static void initialize();
};

#endif // LIB_AFFIR_STOPWORDS_H
