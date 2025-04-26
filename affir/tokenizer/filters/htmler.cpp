#include "htmler.h"
#include <string>

using namespace affir;

void Htmler::process(std::string &token) {
    if(token.empty()) return;
    
    if(token.front() == '<' && token.size() > 1)
        token.clear();
}