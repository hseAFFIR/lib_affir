#include "lowercaser.h"



void Lowercaser::process(std::string& token) {
    std::string result;
    size_t i = 0;
    const size_t token_size = token.size();
    result.reserve(token_size);
    while (i < token_size) {
        // UTF-8 (1-4 bytes)
        int len = 1;
        if ((token[i] & 0x80) != 0) {
            if      ((token[i] & 0xF0) == 0xF0) len = 4;
            else if ((token[i] & 0xE0) == 0xE0) len = 3;
            else if ((token[i] & 0xC0) == 0xC0) len = 2;
        }
        
        std::string_view ch(&token[i], len);

        auto it = lower_map.find(ch);
        if (it != lower_map.end()) {
            result.append(it->second);
        } else {
            // ASCII
            if (len == 1) {
                result.push_back(static_cast<char>(std::tolower(ch[0])));
            } else {
                result.append(ch.data(), ch.size());
            }
        }
        
        i += len;
    }
    token = std::move(result);
    return;
}