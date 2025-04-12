#include "punctuator.h"

void Punctuator::process(std::string &token) {
    if (token.size() == 1 && !std::isalnum(static_cast<unsigned char>(token[0])))
        token.clear();
}
