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
    indexStorage.createIndex(buffer);
    clearBuffer();
}

void Indexer::addToken(const Token &token) {
    const std::string &body = token.getBody();
    unsigned long long fileId = token.getFileId();
    const TokenInfo info{token.getPos(), token.getIndex()};

    // Checking the existence of BigToken with this body
    auto it = buffer.find(body);
    if (it == buffer.end()) {
        BigToken newBT(body);
        newBT.addPosition(fileId, info);

        const size_t newSize = newBT.getSize();
        currentSizeInBytes += newSize;

        Logger::debug("Indexer::addToken", "{} sizeInBytes {}",token.getBody(),currentSizeInBytes);

        if (currentSizeInBytes > maxBufferSizeInBytes) {
            saveTo();
        }

        buffer.emplace(body, std::move(newBT));
    } else {
        BigToken &bt = it->second;
        const size_t oldSize = bt.getSize();

        size_t newSize = oldSize;

        auto btIt = bt.getFilePositions().find(fileId);

        if (btIt == bt.getFilePositions().end()) {
            newSize += sizeof(unsigned long)*3;
        } else {
            newSize += sizeof(unsigned  long)*2;
        }

        currentSizeInBytes += (newSize - oldSize);

        if (currentSizeInBytes > maxBufferSizeInBytes) {
            saveTo();

            buffer[body] = BigToken(body, fileId, info);
            currentSizeInBytes+=buffer[body].getSize();

            Logger::debug("Indexer::addToken", "{} sizeInBytes {}",token.getBody(),currentSizeInBytes);
            return;
        }

        Logger::debug("Indexer::addToken", "{} sizeInBytes {}",token.getBody(),currentSizeInBytes);

        bt.addPosition(fileId, info);
    }
}

const BigToken &Indexer::getTokenInfo(const std::string &tokenName) const {

    auto resultBigToken = new BigToken(tokenName);

    std::vector<PosMap> inputPositions;

    indexStorage.getRawIndex(tokenName, inputPositions);

    for (auto &bt: inputPositions) {
        resultBigToken->mergeFilePositions(bt);
    }

    return *resultBigToken;
}

Indexer::BufferType Indexer::getBufferWithClear() {
    BufferType copy = buffer;
    clearBuffer();
    return copy;
}

const Indexer::BufferType &Indexer::getBuffer() const {
    return buffer;
}

Indexer::~Indexer() {
    if (!buffer.empty()) {
        saveTo();
    }
}
