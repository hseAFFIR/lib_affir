#include "bigToken.h"

size_t BigToken::calculateSize() const {
    size_t size = body.size(); // Размер строки body

    // Размер map[fileId → vector<TokenInfo>]
    for (const auto& pair : filePositions) {
        size += sizeof(unsigned long long); // Ключ fileId
        size += pair.second.size() * sizeof(TokenInfo);
    }

    return size;
}