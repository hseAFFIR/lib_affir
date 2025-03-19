#include "bigToken.h"

size_t BigToken::calculateSize() const {
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

void BigToken::setFilePositions(const std::unordered_map<unsigned long, std::vector<TokenInfo>> &fp) {
    BigToken::filePositions = fp;
}

void BigToken::mergeFilePositions(const std::unordered_map<unsigned long, std::vector<TokenInfo>>& newFilePositions) {
    for (const auto& [fileId, newPositions] : newFilePositions) {
        // Если fileId уже есть, добавляем данные
        if (filePositions.find(fileId) != filePositions.end()) {
            filePositions[fileId].insert(filePositions[fileId].end(), newPositions.begin(), newPositions.end());
        } else {
            // Если fileId нет, просто добавляем новый ключ-значение
            filePositions[fileId] = newPositions;
        }
    }
}

BigToken::~BigToken() {
    filePositions.clear(); // Явное освобождение памяти, если потребуется
}
