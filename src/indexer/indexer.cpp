#include "indexer.h"
#include <vector>
#include <iostream>
#include "../models/bigToken.h"

Indexer::Indexer(unsigned long bufferSize)
        : maxBufferSizeInBytes(bufferSize), currentSizeInBytes(0) {}

void Indexer::clearBuffer() {
    buffer.clear();
    currentSizeInBytes = 0;
    std::cout<<"Buffer cleared"<<std::endl;
}

void Indexer::saveTo() {
    // here will be logic of saving to indexStorage
    clearBuffer();
}

void Indexer::addToken(const Token& token) {
    const std::string& body = token.body;
    unsigned long long fileId = token.fileId;
    TokenInfo info{token.pos, token.wordPos};

    // Проверка существования BigToken с таким body
    auto it = buffer.find(body);
    if (it == buffer.end()) {
        // Создаем новый BigToken
        BigToken newBT(body);
        newBT.addPosition(fileId, info);

        // Добавляем в буфер
        buffer[body] = newBT;

        // Обновляем текущий размер
        size_t newSize = newBT.calculateSize();
        currentSizeInBytes += newSize;
        std::cout << currentSizeInBytes << std::endl;

        // Проверка превышения лимита
        if (currentSizeInBytes > maxBufferSizeInBytes) {
            saveTo(); // Сохраняем и очищаем буфер
        }
    } else {
        // Обновляем существующий BigToken
        BigToken& bt = it->second;
        size_t oldSize = bt.calculateSize();

        bt.addPosition(fileId, info);

        // Обновляем размер
        size_t newSize = bt.calculateSize();
        currentSizeInBytes += (newSize - oldSize);

        std::cout << currentSizeInBytes << std::endl;

        if (currentSizeInBytes > maxBufferSizeInBytes) {
            saveTo(); // Сохраняем и очищаем буфер
        }
    }
}


BigToken Indexer::getTokenInfo(const std::string& tokenName) {
    auto it = buffer.find(tokenName);
    if (it != buffer.end()) {
        return it->second;
    }
    return BigToken(); // Возвращаем пустой объект, если не найден
}

Indexer::BufferType Indexer::getBufferWithClear(){
    clearBuffer();
    return buffer;
}

Indexer::BufferType Indexer::getBuffer() const {
    return buffer;
}
