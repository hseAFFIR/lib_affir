#include <iostream>
#include "tokenizer/filters/filters.h"
#include "processing/dataHandler.h"
#include "storages/indexes/multi/multiFileIndexStorage.h"
#include <vector>
#include "logger/logger.h"

#include <cassert>
#include <fstream>

int main(){
    system("chcp 65001"); // Для корректного отображения русского языка в логах
    Logger::init("logs/log.txt");
    Logger::info("Main", "Application started");
    std::vector<Base*> filters = { new Htmler(), new Punctuator(),
        new StopWords(), new StemFilter()};
    MultiFileIndexStorage storage;
    DataHandler dh(filters, 100, storage);
    // DataHandler dh(100, storage);
    std::string text = "Пройдет много лет, и полковник\n"
                               "Аурелиано Буэндиа, стоя у стены в ожидании расстрела, вспомнит тот далекий вечер, когда отец взял его с собой посмотреть на лед. Макондо было тогда небольшим селением с двумя десятками хижин, выстроенных из глины и бамбука на берегу реки, которая мчала свои прозрачные воды по ложу из белых отполированных камней, огромных, как доисторические яйца.";
    std::string filename = "test_output.txt";
    dh.processText(text, filename);
    for (auto filter : filters) {
        delete filter;
    }
    return 0;
}
