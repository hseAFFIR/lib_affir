#include "lowercaser.h"

std::string Lowercaser::process(const std::string &token) const {
        std::string lowercased = token;
        std::transform(lowercased.begin(), lowercased.end(), lowercased.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return lowercased;
}