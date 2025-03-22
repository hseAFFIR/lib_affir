#include "searcher/search.h"
#include "indexer/indexer.h"
#include "tokenizer/filters/stemFilter.h"
#include <iostream>
#include "logger/logger.h"

int main() {
    system("chcp 65001"); // для корректного отображения русского языка в логах.
    Logger::init("logs/log.txt");
    // Пример использования
    Logger::info("Main", "Application started");

    // Создаем Indexer и добавляем несколько токенов
    Indexer indexer(1000);
    indexer.addToken(Token("example", 100, 5, 1));
    indexer.addToken(Token("test", 200, 10, 2));

    // Создаем StemFilter
    StemFilter stemFilter;

    // Создаем модуль поиска
    Search search(indexer, stemFilter);

    // Выполняем поиск
    auto results = search.find("Пройдет много лет, и полковник\n"
                               "Аурелиано Буэндиа, стоя у стены в ожидании расстрела, вспомнит тот далекий вечер, когда отец взял его с собой посмотреть на лед. Макондо было тогда небольшим селением с двумя десятками хижин, выстроенных из глины и бамбука на берегу реки, которая мчала свои прозрачные воды по ложу из белых отполированных камней, огромных, как доисторические яйца.");


//    auto results = search.find("");

    // Выводим результаты
    for (const auto& result : results) {
        std::cout << "File ID: " << result.first << ", Position: " << result.second << std::endl;
    }

    return 0;
}