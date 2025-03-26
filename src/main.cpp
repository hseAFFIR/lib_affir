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

    // Данные для индексации
    std::unordered_map<std::string, BigToken> data;

    // Создаём BigToken для "example"
    BigToken token1("example");
    token1.addPosition(1, {100, 10});
    token1.addPosition(1, {150, 15});
    token1.addPosition(2, {200, 20});

    // Создаём BigToken для "test"
    BigToken token2("test");
    token2.addPosition(3, {50, 5});
    token2.addPosition(3, {75, 7});

    // Добавляем в структуру данных
    data["example"] = token1;
    data["test"] = token2;

    // Записываем в индекс
    std::cout << "Создаём индекс..." << std::endl;
    storage.createIndex(data);

    // Проверяем, что файлы создались
    std::cout << "\nПроверьте файлы в каталоге!" << std::endl;
    system("ls -l");

    // Запрашиваем индекс для "example"
    std::cout << "\nПолучаем индекс для 'example':" << std::endl;
    std::vector<PosMap> results;
    storage.getRawIndex("example", results);

    // Вывод результатов
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << "Файл #" << (i + 1) << " содержит:" << std::endl;
        for (const auto& [fileId, positions] : results[i]) {
            std::cout << "  В файле " << fileId << " позиции: ";
            for (const auto& pos : positions) {
                std::cout << "(" << pos.pos << ", " << pos.wordPos << ") ";
            }
            std::cout << std::endl;
        }
    }


//    // Создаем Indexer и добавляем несколько токенов
//    Indexer indexer(1000);
//    indexer.addToken(Token("example", 100, 5, 1));
//    indexer.addToken(Token("test", 200, 10, 2));
//
//    // Создаем StemFilter
//    StemFilter stemFilter;
//
//    // Создаем модуль поиска
//    Search search(indexer, stemFilter);
//
//    // Выполняем поиск
//    auto results2 = search.find("Пройдет много лет, и полковник\n"
//                               "Аурелиано Буэндиа, стоя у стены в ожидании расстрела, вспомнит тот далекий вечер, когда отец взял его с собой посмотреть на лед. Макондо было тогда небольшим селением с двумя десятками хижин, выстроенных из глины и бамбука на берегу реки, которая мчала свои прозрачные воды по ложу из белых отполированных камней, огромных, как доисторические яйца.");
//
//    // Выводим результаты
//    for (const auto& result : results2) {
//        std::cout << "File ID: " << result.first << ", Position: " << result.second << std::endl;
//    }

    return 0;
}