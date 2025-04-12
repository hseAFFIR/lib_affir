#include "lowercaser.h"

void Lowercaser::process(std::string& token) {
        std::transform(token.begin(), token.end(), token.begin(),
                       [](unsigned char c) { return std::tolower(c); });
}