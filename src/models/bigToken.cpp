#include "bigToken.h"

BigToken::BigToken(std::string body, unsigned long fileId, TokenInfo info): body(std::move(body)) {
    std::vector<TokenInfo> vInfo;
    vInfo.push_back(std::move(info));
    filePositions.emplace(std::move(fileId), std::move(vInfo));
}

const PosMap &BigToken::getFilePositions() const {
    return filePositions;
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
