#ifndef LIB_AFFIR_LOWERCASER_H
#define LIB_AFFIR_LOWERCASER_H

#include <algorithm>
#include <cctype>
#include <unordered_map>
#include "base.h"

namespace affir {

/**
 * @brief The Lowercaser class converts all characters of a token to lowercase.
 *
 * This filter inherits from Base and implements the process method to transform the input token
 * so that all its characters are in lowercase. It is typically used to normalize text before further processing.
 */
    class Lowercaser : public Base {
    public:
        /**
         * @brief Converts the input token to lowercase.
         *
         * This method creates a copy of the input token and applies std::transform along with std::tolower
         * to convert each character to its lowercase equivalent.
         *
         * @param token The input token to be processed.
         * @return std::string The resulting token with all characters converted to lowercase.
         */
        void process(std::string &token) override;

        /**
         * @brief Returns the processing order for the Lowercaser filter.
         *
         * This method specifies the order in which the Lowercaser filter should be applied relative to other filters.
         *
         * @return FilterOrder The order value corresponding to Lowercaser.
         */
        FilterOrder getOrder() const override {
            return FilterOrder::Lowercaser;
        }

    private:
        // This table should include all symbols that are unable
        // to be lowercased  by the algorithm
        const std::unordered_map<std::string_view, std::string_view> lower_map = {};
    };
}

#endif // LIB_AFFIR_LOWERCASER_H