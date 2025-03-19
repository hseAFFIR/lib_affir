#include "bigToken.h"

const size_t BigToken::calculateSize() const {
    size_t size = body.size(); // Размер строки body

    for (const auto& pair : filePositions) {
        size += sizeof(unsigned long long); // Ключ fileId
        size += pair.second.size() * sizeof(TokenInfo);
    }

    return size;
}

const std::unordered_map<unsigned long, std::vector<TokenInfo>> &BigToken::getFilePositions() const {
    return filePositions;
}

void BigToken::setFilePositions(const std::unordered_map<unsigned long, std::vector<TokenInfo>> &filePositions) {
    BigToken::filePositions = filePositions;
}
