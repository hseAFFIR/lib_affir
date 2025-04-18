#include "indexer.h"
#include "quill/LogMacros.h"
#include "../logger/logger.h"

#include <iostream>


#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/LogMacros.h"
#include "quill/Logger.h"
#include "quill/sinks/ConsoleSink.h"
#include <string_view>

//quill::Logger* logger = quill::Frontend::get_logger("root");

Indexer::Indexer(size_t bufferSize, IIndexStorage &indStor)
        : maxBufferSizeInBytes(bufferSize), currentSizeInBytes(0), indexStorage(indStor) {
    logger = GetRootLogger();
        LOG_INFO(logger,"Indexer module initialized");
}


void Indexer::clearBuffer() {
    buffer.clear();
    currentSizeInBytes = 0;

//    Logger::debug("Indexer", "Buffer cleared");
    LOG_DEBUG( logger,"Buffer cleared");



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

        currentSizeInBytes += newBT.getPosesSize();
        buffer.emplace(token.body, std::move(newBT));
    } else {
        BigToken &bt = it->second;
        const size_t oldSize = bt.getPosesSize();
        size_t newSize = oldSize;

        if (!bt.getFilePositions().contains(token.fileId))
            newSize += sizeof(FileId) + sizeof(TokenInfo);
        else
            newSize += sizeof(TokenInfo);

        if (currentSizeInBytes + (newSize - oldSize) > maxBufferSizeInBytes) {
            saveTo();
            addToken(token);
            return;
        }
        bt.addPosition(token.fileId, token.info);
        currentSizeInBytes += (newSize - oldSize);
    }
//    Logger::debug("Indexer (addToken)", "{} sizeInBytes {}", token.body, currentSizeInBytes);

    LOG_DEBUG( logger,"Indexer (addToken)", "{} sizeInBytes {}", token.body, currentSizeInBytes);


}

BigToken Indexer::getTokenInfo(const std::string &tokenName) {

    BigToken resultBigToken(tokenName);
    resultBigToken.mergeFilePositions(indexStorage.getRawIndex(tokenName));

    return resultBigToken;
}

Indexer::~Indexer() {
    saveTo();
}
