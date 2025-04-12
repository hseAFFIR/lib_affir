#include "indexer.h"
#include <iostream>
#include "../logger/logger.h"

Indexer::Indexer(unsigned long bufferSize, IIndexStorage &indStor)
        : maxBufferSizeInBytes(bufferSize), currentSizeInBytes(0), indexStorage(indStor) {
    Logger::info("Indexer", "Indexer module initialized");
}

void Indexer::clearBuffer() {
    buffer.clear();
    currentSizeInBytes = 0;
    Logger::debug("Indexer", "Buffer cleared");
}

void Indexer::saveTo() {
    if (!buffer.empty())
        indexStorage.createIndex(buffer);
    clearBuffer();
}

void Indexer::addToken(const Token &token) {
    // Checking the existence of BigToken with this body
    auto it = buffer.find(token.body);
    if (it == buffer.end()) {
        BigToken newBT(token);

        if (currentSizeInBytes + newBT.getPosesSize() > maxBufferSizeInBytes)
            saveTo();

        buffer.emplace(token.body, std::move(newBT));
        currentSizeInBytes = newBT.getPosesSize();
    } else {
        BigToken &bt = it->second;
        size_t newSize = bt.getPosesSize();

        if (!bt.getFilePositions().contains(token.fileId))
            newSize += sizeof(FileId) + sizeof(TokenInfo);
        else
            newSize += sizeof(TokenInfo);

        if (newSize > maxBufferSizeInBytes) {
            saveTo();
            addToken(token);
            return;
        }
        bt.addPosition(token.fileId, token.info);
    }
    Logger::debug("Indexer (addToken)", "{} sizeInBytes {}", token.body, currentSizeInBytes);
}

BigToken Indexer::getTokenInfo(const std::string &tokenName) {

    BigToken resultBigToken(tokenName);
    resultBigToken.mergeFilePositions(indexStorage.getRawIndex(tokenName));

    return resultBigToken;
}

Indexer::~Indexer() {
    saveTo();
}
