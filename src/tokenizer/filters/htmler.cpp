#include "htmler.h"
#include <string>

std::string Htmler::process(const std::string &token) const {
    if(token.empty()) return token;
    
    if(token.front() == '<' && token.size() > 1) {
        return "";
    }
    return token;
}