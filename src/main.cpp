#include "searcher/search.h"
#include "indexer/indexer.h"
#include "tokenizer/filters/stemFilter.h"
#include <iostream>
#include "logger/logger.h"
#include "storages/multiFileIndexStorage.h"

int main() {
    // Logger
    system("chcp 65001"); // Для корректного отображения русского языка в логах
    Logger::init("logs/log.txt");
    Logger::info("Main", "Application started");

    MultiFileIndexStorage storage;

    Indexer indexer(75, storage);

    indexer.addToken(Token("example", 100, 10, 1));
    indexer.addToken(Token("example", 150, 15, 1));
    indexer.addToken(Token("example", 200, 20, 2));
    indexer.addToken(Token("example", 201, 109, 3));
    indexer.addToken(Token("example", 201, 109, 1));

    indexer.addToken(Token("test", 50, 5, 3));
    indexer.addToken(Token("test", 75, 7, 3));

    indexer.saveTo();


    storage.saveMetadata();

    std::cout << "\nПолучаем индекс для 'example':" << std::endl;
    std::vector<PosMap> results;
    storage.getRawIndex("example", results);

    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << "Файл #" << (i + 1) << " содержит:" << std::endl;
        for (const auto &[fileId, positions]: results[i]) {
            std::cout << "  В файле " << fileId << " позиции: ";
            for (const auto &pos: positions) {
                std::cout << "(" << pos.pos << ", " << pos.wordPos << ") ";
            }
            std::cout << std::endl;
        }
    }

    std::cout << "\n\nПроверка getTokenInfo\n";
    BigToken resultBt = indexer.getTokenInfo("example");
    PosMap tokenPos = resultBt.getFilePositions();
    for (const auto &[fileId, positions]: tokenPos) {
        std::cout << "  В файле " << fileId << " позиции: ";
        for (const auto &pos: positions) {
            std::cout << "(" << pos.pos << ", " << pos.wordPos << ") ";
        }
        std::cout << std::endl;
    }


    indexer.addToken(Token("example", 100, 5, 1));
    indexer.addToken(Token("test", 200, 10, 2));

    // Создаем StemFilter
    StemFilter stemFilter;

    // Создаем модуль поиска
    Search search(indexer, stemFilter);

    // Выполняем поиск
    auto results2 = search.find("Пройдет много лет, и полковник\n"
                               "Аурелиано Буэндиа, стоя у стены в ожидании расстрела, вспомнит тот далекий вечер, когда отец взял его с собой посмотреть на лед. Макондо было тогда небольшим селением с двумя десятками хижин, выстроенных из глины и бамбука на берегу реки, которая мчала свои прозрачные воды по ложу из белых отполированных камней, огромных, как доисторические яйца.");

    // Выводим результаты
    for (const auto& result : results2) {
        std::cout << "File ID: " << result.first << ", Position: " << result.second << std::endl;
    }

    return 0;
}