#include "bigToken.h"

size_t BigToken::calculateSize() const {
    size_t size = body.size(); // Размер строки body

    // sizeof(token_body) + ( sizeof(FileId) + sizeof(TokenInfo) * tokens_num ) * vector.size()
    return size + posMapSize;
}

const PosMap &BigToken::getFilePositions() const {
    return filePositions;
}

void BigToken::setFilePositions(const PosMap &fp) {
    BigToken::filePositions = fp;
}

void BigToken::mergeFilePositions(const PosMap& newFilePositions) {
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

void BigToken::addPosition(FileId fileId, const TokenInfo &info) {
    filePositions[fileId].push_back(info);
    posMapSize += sizeof(FileId) + sizeof(TokenInfo);
}
