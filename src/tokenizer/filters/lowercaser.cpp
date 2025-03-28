#include "lowercaser.h"



std::string Lowercaser::process(const std::string& token) const {
    std::string result;
    size_t i = 0;
    while (i < token.size()) {
        // Определяем длину символа UTF-8 (1-4 байта)
        int len = 1;
        if ((token[i] & 0x80) != 0) {
            if      ((token[i] & 0xF0) == 0xF0) len = 4;
            else if ((token[i] & 0xE0) == 0xE0) len = 3;
            else if ((token[i] & 0xC0) == 0xC0) len = 2;
        }
        
        // Извлекаем символ
        std::string ch = token.substr(i, len);
        
        // Ищем в таблице замен
        auto it = lower_map.find(ch);
        if (it != lower_map.end()) {
            result += it->second;
        } else {
            // Для латиницы используем tolower()
            if (len == 1) {
                result += static_cast<char>(std::tolower(ch[0]));
            } else {
                result += ch;
            }
        }
        
        i += len;
    }
    return result;
}