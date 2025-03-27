#include "indexer.h"
#include <iostream>
#include "../models/token.h"
#include "../logger/logger.h"

Indexer::Indexer(unsigned long bufferSize)
        : maxBufferSizeInBytes(bufferSize), currentSizeInBytes(0) {
    Logger::info("Indexer", "Indexer module initialized");
}

void Indexer::clearBuffer() {
    buffer.clear();
    currentSizeInBytes = 0;
    Logger::debug("Indexer", "Buffer cleared");
}

void Indexer::saveTo() {
    // Logic for saving to index storage will be here
    clearBuffer();
}

void Indexer::addToken(const Token &token) {
    const std::string &body = token.getBody();
    unsigned long long fileId = token.getFileId();
    const TokenInfo info{token.getPos(), token.getIndex()};

    // Проверка существования BigToken с таким body
    auto it = buffer.find(body);
    if (it == buffer.end()) {
        // Создаем новый BigToken
        BigToken newBT(body);
        newBT.addPosition(fileId, info);

        // Добавляем в буфер
        buffer.emplace(body, std::move(newBT));

        // Обновляем текущий размер
        const size_t newSize = buffer[body].getSize();
        currentSizeInBytes += newSize;

        Logger::debug("Indexer::addToken", "{} sizeInBytes {}",token.getBody(),currentSizeInBytes);
        // Проверка превышения лимита
        if (currentSizeInBytes > maxBufferSizeInBytes) {
            saveTo(); // Сохраняем и очищаем буфер
        }
    } else {
        // Обновляем существующий BigToken
        BigToken &bt = it->second;
        const size_t oldSize = bt.getSize();

        bt.addPosition(fileId, info);

        // Обновляем размер
        const size_t newSize = bt.getSize();
        currentSizeInBytes += (newSize - oldSize);

        Logger::debug("Indexer::addToken", "{} sizeInBytes {}",token.getBody(),currentSizeInBytes);

        if (currentSizeInBytes > maxBufferSizeInBytes) {
            saveTo(); // Сохраняем и очищаем буфер
        }
    }
}

const BigToken &Indexer::getTokenInfo(const std::string &tokenName) const {

    std::vector<BigToken *> inputBigTokens;
    BigToken token("example");

    // Добавляем позиции для разных файлов
    token.addPosition(1, {100, 5});
    token.addPosition(1, {150, 7});
    token.addPosition(2, {200, 10});
    inputBigTokens.push_back(&token);

    BigToken token2("example");

    // Добавляем позиции для разных файлов
    token.addPosition(3, {100, 5});
    token.addPosition(3, {150, 7});
    token.addPosition(4, {200, 10});
    inputBigTokens.push_back(&token2);

    auto resultBigToken = new BigToken(tokenName);

    for (auto &bt: inputBigTokens) {
        resultBigToken->mergeFilePositions(bt->getFilePositions());
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
        saveTo(); // Сохранение буфера перед уничтожением
    }
}
