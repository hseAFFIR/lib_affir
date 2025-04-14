#include "storages/indexes/multi/multiFileIndexStorage.h"
#include "indexer/indexer.h"
#include "searcher/search.h"
#include "logger/logger.h"
#include "processing/dataHandler.h"
#include "tokenizer/filters/filters.h"

#include <cassert>
#include <fstream>
#include "tokenizer/filters/russianPorterStemmer.h"


int main(){
    system("chcp 65001");
    Logger::init("logs/log.txt");
    Logger::info("Main", "Application started");
    MultiFileIndexStorage storage;
    std::vector<Base*> filters = {};
    DataHandler dh(filters,100, storage);
    std::string text = "Привет, мир!";
    RussianPorterStemmer rps;
        
    std::string testtexttt = "АБВГДЕЁЖЗИКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    std::string testtext = "Hello, World!";
    
    std::string filename = "test_output.txt";
    dh.processText(testtexttt, filename);
    for (auto filter : filters) {
        delete filter;
    }
    return 0;
}
