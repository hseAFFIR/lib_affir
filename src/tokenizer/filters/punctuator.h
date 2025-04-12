#ifndef LIB_AFFIR_PUNCTUATOR_H
#define LIB_AFFIR_PUNCTUATOR_H

#include "base.h"
#include <cctype>

/**
 * @brief The Punctuator filter processes tokens to remove punctuation.
 *
 * This filter checks if a token consists of a single character and if that character
 * is not alphanumeric. In such a case, it returns an empty string, effectively removing
 * standalone punctuation from the token stream.
 */

class Punctuator : public Base {
public:
    /**
     * @brief Processes the token to filter out punctuation.
     *
     * If the token is exactly one character long and that character is not alphanumeric,
     * the method returns an empty string, otherwise it returns the token unchanged.
     *
     * @param token The input token to process.
     * @return std::string The processed token, or an empty string if the token is punctuation.
     */
    void process(std::string &token) override;

    /**
     * @brief Returns the processing order for the Punctuator filter.
     *
     * This method specifies the order in which the Punctuator filter should be applied relative to other filters.
     *
     * @return FilterOrder The order value corresponding to Punctuator.
     */
    FilterOrder getOrder() const override {
        return FilterOrder::Punctuator;
    }
};

#endif // LIB_AFFIR_PUNCTUATOR_H