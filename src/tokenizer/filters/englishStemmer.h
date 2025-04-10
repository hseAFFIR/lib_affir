#ifndef ENGLISHSTEMMER_H
#define ENGLISHSTEMMER_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>
#include "base.h"

class EnglishStemmer : public Base {
public:
    /**
     * @brief Implements the English stemming algorithm (Porter2).
     *
     * This class provides a method to process an English word and returns its stemmed form according to the Porter2 algorithm.
     */

    /**
     * @brief Stems the given English word using the Porter2 algorithm.
     *
     * The input word is expected to be in lower case without unnecessary characters.
     *
     * @param word The input word to be stemmed.
     * @return std::string The stemmed (reduced) version of the input word.
     */
    std::string process(const std::string &word) const override;

    /**
     * @brief Constructs an EnglishStemmer object.
     *
     * The constructor initializes all constants and exception tables needed for the stemming process.
     */
    EnglishStemmer();

    /**
     * @brief Returns the filter order associated with the EnglishStemmer.
     *
     * This method provides a fixed order for the filter to ensure proper sorting.
     *
     * @return FilterOrder The order of the EnglishStemmer filter.
     */
    FilterOrder getOrder() const override {
        return FilterOrder::EnglishStemmer;
    }

private:
    std::string english_vowels; ///< A string containing English vowel characters.
    std::vector<std::string> english_doubles; ///< A list of double consonant strings used in the algorithm.
    std::string english_valid_li_endings; ///< Valid endings for "li" deletion rule.
    std::unordered_map<std::string, std::string> english_exceptions1; ///< Exception mappings for specific words.
    std::unordered_set<std::string> english_exceptions2; ///< A set of words that are exceptions to certain stemming rules.

    /**
     * @brief Determines the regions R1 and R2 for the input word.
     *
     * R1 is the region after the first non-vowel following a vowel and R2 is the region after
     * the first non-vowel following a vowel in R1. Special handling is applied for certain prefixes.
     *
     * @param word The word for which regions are to be determined.
     * @return std::pair<long long, long long> A pair containing R1 and R2.
     */
    std::pair<long long, long long> english_set_regions(const std::string &word) const;
    
    /**
     * @brief Applies suffix replacement rules to the input word.
     *
     * This method executes steps 0, 1a, and 1c of the Porter2 algorithm by removing or replacing
     * certain suffixes from the word.
     *
     * @param word The word to process for suffix replacement.
     * @return std::string The word after suffix replacement.
     */
    std::string english_replace_suffixes(std::string &word) const;
    
    /**
     * @brief Checks if the word is considered "short" based on its syllable structure and region R1.
     *
     * @param word The word (or syllable) to check.
     * @param R1 The region R1 of the word.
     * @return true if the word is short, false otherwise.
     */
    bool english_is_short(const std::string &word, const long long &R1) const;

    /**
     * @brief Determines if a given word segment forms a short syllable.
     *
     * A short syllable is defined by specific vowel and consonant patterns.
     *
     * @param word The word segment to analyze.
     * @return true if the segment is a short syllable, false otherwise.
     */
    bool english_is_short_syllable(const std::string &word) const;

    /**
     * @brief Executes step 2 of the Porter2 algorithm for suffix replacement.
     *
     * @param word The input word.
     * @param R1 The region R1 of the word.
     * @return std::string The word after processing step 2.
     */
    std::string step2(std::string &word, const long long &R1) const;

    /**
     * @brief Executes step 3 of the Porter2 algorithm for suffix replacement.
     *
     * @param word The input word.
     * @param R1 The region R1 of the word.
     * @param R2 The region R2 of the word.
     * @return std::string The word after processing step 3.
     */
    std::string step3(std::string &word, const long long &R1, const long long &R2) const;

    /**
     * @brief Executes step 4 of the Porter2 algorithm for suffix removal.
     *
     * @param word The input word.
     * @param R2 The region R2 of the word.
     * @return std::string The word after processing step 4.
     */
    std::string step4(std::string &word, const long long &R2) const;

    /**
     * @brief Executes step 5 of the Porter2 algorithm for final adjustments.
     *
     * This step may remove a trailing 'e' or 'l' under specific conditions.
     *
     * @param word The input word.
     * @param R1 The region R1 of the word.
     * @param R2 The region R2 of the word.
     * @return std::string The final stemmed word after processing step 5.
     */
    std::string step5(std::string &word, const long long &R1, const long long &R2) const;

    /**
     * @brief Checks if the string ends with the given suffix.
     *
     * @param s The string to check.
     * @param suffix The suffix to compare with.
     * @return true if s ends with suffix, false otherwise.
     */
    static bool ends_with(const std::string &s, const std::string &suffix);

    /**
     * @brief Determines if any character in the string s is contained in the string chars.
     *
     * @param s The string to search.
     * @param chars The string containing characters to search for.
     * @return true if any character from chars is found in s, false otherwise.
     */
    static bool contains_any(const std::string &s, const std::string &chars);

    /**
     * @brief Converts the input string to lower case.
     *
     * @param s The string to convert.
     * @return std::string The lower-case version of s.
     */
    static std::string to_lower(const std::string &s);
};

#endif // ENGLISHSTEMMER_H
