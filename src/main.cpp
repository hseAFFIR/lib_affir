#include <iostream>
#include "searcher/search.h"
#include "indexer/indexer.h"
#include "tokenizer/filters/stemFilter.h"
#include "logger/logger.h"
#include "storages/indexes/multi/multiFileIndexStorage.h"

int main() {
    // Настройка
    system("chcp 65001");
    Logger::init("logs/log.txt");
    Logger::info("Main", "Запуск");

    MultiFileIndexStorage storage;
    Indexer indexer(1024, storage);
    StemFilter stemFilter;

    // Добавляем токены
    indexer.addToken(Token("полковник", 100, 1, 1));  // pos=100, wordPos=1, fileId=1
    indexer.addToken(Token("ива", 110, 2, 1)); // pos=110, wordPos=2
    indexer.addToken(Token("ы", 200, 3, 1));
    indexer.addToken(Token("иваны", 300, 1, 2)); // Другая форма в fileId=2

    // Сохраняем
    indexer.saveTo();
    storage.saveMetadata();

    // Проверка индекса
    std::string searchWord = "полковник";
    BigToken bt = indexer.getTokenInfo(searchWord);
    std::cout << "Токен " <<searchWord<< "найден в " << bt.getFilePositions().size() << " файлах\n";

    // Поиск
    Search search(indexer, stemFilter);

    auto results = search.find("\"полковники иваны\"");
    if (results.empty()) {
        std::cout << "Фраза не найдена" << std::endl;
    } else {
        std::cout << "\n=== Результаты поиска фразы ===" << std::endl;
        for (const auto& [fileId, positions] : results) {
            std::cout << "Файл ID: " << fileId << std::endl;
            std::cout << "Найденные позиции:" << std::endl;

            for (const auto& info : positions) {
                std::cout << "  - Абсолютная позиция: " << info.pos
                          << ", Относительная позиция: " << info.wordPos << std::endl;
            }

            // Если нужно вывести само содержимое из файла:
            // std::string fileContent = storage.getFileContent(fileId);
            // std::cout << "Контекст: " << fileContent.substr(info.pos, 50) << "..." << std::endl;
        }
    }
    return 0;
}