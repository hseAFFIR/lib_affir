#include "lowercaser.h"

// This table should include all symbols that are unable
// to be lowercased  by the algorithm
namespace {
    const std::unordered_map<std::string_view, std::string_view> lower_map = {
    };
}

#include "lowercaser.h"

void Lowercaser::process(std::string& token) {
    std::string result;
    result.reserve(token.size());
    const size_t size = token.size();
    size_t i = 0;
    while (i < size) {
        const unsigned char c1 = static_cast<unsigned char>(token[i]);
        // ASCII
        if ((c1 & 0x80) == 0) {
            result += (c1 >= 'A' && c1 <= 'Z') ? c1 + 32 : c1;
            i++;
            continue;
        }

        // UTF-8 Cyrillic
        if (i + 1 < size) {
            const unsigned char c2 = static_cast<unsigned char>(token[i + 1]);

            // All letters (except for Ё)
            if (c1 == 0xD0 && c2 >= 0x90 && c2 <= 0x9F) {
                result += static_cast<char>(0xD0);
                result += static_cast<char>(c2 + 0x20);
                i += 2;
                continue;
            }

            if (c1 == 0xD0 && c2 >= 0xA0 && c2 <= 0xAF) {
                result += static_cast<char>(0xD1);
                result += static_cast<char>(c2 - 0x20);
                i += 2;
                continue;
            }


            // Ё (D0 81 -> D1 91)
            if (c1 == 0xD0 && c2 == 0x81) {
                result += static_cast<char>(0xD1);
                result += static_cast<char>(0x91);
                i += 2;
                continue;
            }
        }

        // The rest
        int len = 2;
        if     ((c1 & 0xF0) == 0xF0) len = 4;
        else if ((c1 & 0xE0) == 0xE0) len = 3;

        len = (i + len > size) ? 1 : len;
        std::string_view ch(&token[i], len);
        if (auto it = lower_map.find(ch); it != lower_map.end()) {
            result += it->second;
        } else {
            result += ch;
        }

        i += len;
    }

    token = std::move(result);
}