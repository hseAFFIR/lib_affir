#ifndef LIB_AFFIR_HTMLER_H
#define LIB_AFFIR_HTMLER_H

#include "base.h"

/**
 * @brief The Htmler class is a filter that processes tokens to remove HTML-like content.
 *
 * This class derives from Base and implements a simple filter that checks if a token
 * starts with the '<' character (suggesting an HTML tag). If so, it returns an empty string;
 * otherwise, it returns the token unchanged.
 */
class Htmler : public Base {
public:
    /**
     * @brief Processes a token by removing HTML tags.
     *
     * This method checks if the token starts with the '<' character and has more than one character.
     * If both conditions are met, the token is considered to be an HTML tag and an empty string is returned.
     * Otherwise, the original token is returned.
     *
     * @param token The input token to be processed.
     * @return std::string The processed token, or an empty string if the token is identified as HTML.
     */
    void process(std::string &token) override;
    
    /**
     * @brief Returns the processing order for the Htmler filter.
     *
     * This method specifies the order in which the Htmler filter should be applied relative to other filters.
     *
     * @return FilterOrder The filter order value corresponding to Htmler.
     */
    FilterOrder getOrder() const override {
        return FilterOrder::Htmler;
    }
};

#endif // LIB_AFFIR_HTMLER_H