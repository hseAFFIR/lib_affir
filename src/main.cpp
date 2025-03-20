#include <iostream>
#include "indexer/indexer.h"
#include "storages/fileStorage.h"
#include "models/token.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    auto *indexer1 = new Indexer(100);

    auto *token1 = new Token("apple", 1, 200, 1);
    auto *token2 = new Token("banana", 2, 300, 2);
    auto *token3 = new Token("apple2", 1, 400, 3);


    std::cout << "Adding tokens..." << std::endl;
    for (int i = 0; i < 2; ++i) {

        indexer1->addToken(*token1);
        indexer1->addToken(*token2);
        indexer1->addToken(*token3);
    }

    std::cout << "\nChecking buffer after overflow:" << std::endl;
    auto buffer = indexer1->getBuffer();
    std::cout << "Current buffer size: " << buffer.size() << " entries" << std::endl;


    // Создаем объект BigToken
    BigToken token("example");

    // Добавляем позиции для разных файлов
    token.addPosition(1, {100, 5});
    token.addPosition(1, {150, 7});
    token.addPosition(2, {200, 10});

    // Выводим начальные позиции
    std::cout << "Initial positions:\n";
    for (const auto& [fileId, positions] : token.getFilePositions()) {
        std::cout << "File ID: " << fileId << " -> Positions: ";
        for (const auto& pos : positions) {
            std::cout << "(" << pos.pos << ", " << pos.wordPos << ") ";
        }
        std::cout << std::endl;
    }

    // Создаем новую структуру данных для слияния
    std::unordered_map<unsigned long, std::vector<TokenInfo>> newPositions = {
            {1, {{170, 8}, {180, 9}}},  // Дополняем существующий файл ID 1
            {3, {{300, 15}, {310, 16}}} // Добавляем новый файл ID 3
    };

    // Вызываем addPosition с новой структурой
    token.mergeFilePositions(newPositions);

    std::cout << "\nAfter addPosition:\n";
    for (const auto& [fileId, positions] : token.getFilePositions()) {
        std::cout << "File ID: " << fileId << " -> Positions: ";
        for (const auto& pos : positions) {
            std::cout << "(" << pos.pos << ", " << pos.wordPos << ") ";
        }
        std::cout << std::endl;
    }

    // Создаем другую структуру данных для mergeFilePositions
    std::unordered_map<unsigned long, std::vector<TokenInfo>> mergeData = {
            {2, {{220, 11}}},          // Дополняем файл ID 2
            {4, {{400, 20}, {410, 21}}} // Новый файл ID 4
    };

    // Вызываем mergeFilePositions
    token.mergeFilePositions(mergeData);

    std::cout << "\nAfter mergeFilePositions:\n";
    for (const auto& [fileId, positions] : token.getFilePositions()) {
        std::cout << "File ID: " << fileId << " -> Positions: ";
        for (const auto& pos : positions) {
            std::cout << "(" << pos.pos << ", " << pos.wordPos << ") ";
        }
        std::cout << std::endl;
    }


    BigToken bigToken = indexer1->getTokenInfo("example");

    std::cout << "\nGet token Info:\n";
    for (const auto& [fileId, positions] : bigToken.getFilePositions()) {
        std::cout << "File ID: " << fileId << " -> Positions: ";
        for (const auto& pos : positions) {
            std::cout << "(" << pos.pos << ", " << pos.wordPos << ") ";
        }
        std::cout << std::endl;
    }


//    std::string str = "1234,5687,(),1234,4567,()";
//    std::string str2 = "asdf,1234,asdf,ery,()1234";
//    auto *fileStorage = new FileStorage("MyFile", str.size());
//    auto *fileStorage2 = new FileStorage("MyFile", str2.size());
//    fileStorage2->write(str2);
//    fileStorage->write(str);
//    fileStorage->close();
//    fileStorage2->close();

//    auto *fileStorage3 = new FileStorage(1);
//
//    std::vector<char> buffer;
//    while (!fileStorage3->isEnd()) {
//        fileStorage3->read(buffer);
//        std::cout << buffer.data() << std::endl;
//    }
//    FileStorage::saveStorageMeta();
    // Token token("Hello",1,2,1);
    // std::cout<<token<<std::endl;
    // Token token2("Hello",1,2,1);
    // if (token==token2) {
    //     std::cout<<"YES"<<std::endl;
    // }
    // else {
    //     std::cout<<"NO"<<std::endl;
    //
    // }
    return 0;
}