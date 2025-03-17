#include "indexer.h"
#include <iostream>

Indexer::Indexer(unsigned long bufferSize)
        : maxBufferSizeInBytes(bufferSize), currentSizeInBytes(0) {}

void Indexer::clearBuffer() {
    buffer.clear();
    currentSizeInBytes = 0;
    std::cout << "Buffer cleared" << std::endl;
}

void Indexer::saveTo() {
    // Logic for saving to index storage will be here
    clearBuffer();
}

void Indexer::addToken(const Token& token) {
    const std::string& body = token.body;
    unsigned long long fileId = token.fileId;
    const TokenInfo info{token.pos, token.wordPos};

    // Проверка существования BigToken с таким body
    auto it = buffer.find(body);
    if (it == buffer.end()) {
        // Создаем новый BigToken
        BigToken newBT(body);
        newBT.addPosition(fileId, info);

        // Добавляем в буфер
        buffer.emplace(body, std::move(newBT));

        // Обновляем текущий размер
        const size_t newSize = buffer[body].calculateSize();
        currentSizeInBytes += newSize;
        std::cout <<token.body<<"   "<< currentSizeInBytes << std::endl;

        // Проверка превышения лимита
        if (currentSizeInBytes > maxBufferSizeInBytes) {
            saveTo(); // Сохраняем и очищаем буфер
        }
    } else {
        // Обновляем существующий BigToken
        BigToken& bt = it->second;
        const size_t oldSize = bt.calculateSize();

        bt.addPosition(fileId, info);

        // Обновляем размер
        const size_t newSize = bt.calculateSize();
        currentSizeInBytes += (newSize - oldSize);

        std::cout <<token.body<<"   "<< currentSizeInBytes << std::endl;

        if (currentSizeInBytes > maxBufferSizeInBytes) {
            saveTo(); // Сохраняем и очищаем буфер
        }
    }
}

const BigToken* Indexer::getTokenInfo(const std::string& tokenName) const {
    auto it = buffer.find(tokenName);
    return (it != buffer.end()) ? &it->second : nullptr;
}

Indexer::BufferType Indexer::getBufferWithClear() {
    BufferType copy = buffer;
    clearBuffer();
    return copy;
}

const Indexer::BufferType& Indexer::getBuffer() const {
    return buffer;
}
