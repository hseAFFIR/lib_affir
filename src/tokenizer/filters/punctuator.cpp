#include "punctuator.h"

std::string Punctuator::process(std::string& token) {
    if (token.size() == 1 && !std::isalnum(static_cast<unsigned char>(token[0]))) {
        return "";
    }
    return token;
}
