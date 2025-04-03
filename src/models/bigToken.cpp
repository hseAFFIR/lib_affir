#include "bigToken.h"

BigToken::BigToken(std::string body, unsigned long fileId, TokenInfo info): body(std::move(body)) {
    std::vector<TokenInfo> vInfo;
    vInfo.push_back(std::move(info));
    filePositions.emplace(std::move(fileId), std::move(vInfo));
};

size_t BigToken::calculateSize() const {
    size_t size = body.size(); // Размер строки body

    for (const auto& pair : filePositions) {
        size += sizeof(unsigned long long); // Ключ fileId
        size += pair.second.size() * sizeof(TokenInfo);
    }

    return size;
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
