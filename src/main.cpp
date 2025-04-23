#include <iostream>
#include <tuple>
#include <fstream>
#include <chrono>
#include "tokenizer/filters/filters.h"
#include "searcher/search.h"
#include "storages/indexes/multi/multiFileIndexStorage.h"
#include "processing/dataHandler.h"

int main() {
    system("chcp 65001");
    Logger::init("info","");
    std::vector<Base*> filters = { new Htmler(), new Punctuator(),
                                   new StopWords()};

    MultiFileIndexStorage storage;
    DataHandler dataHandler(filters, 1024*1024*16, storage);

    std::string str = "This ebook is for the use of anyone anywhere in the United States and\n"
                      "most other parts of the world at no cost and with almost no restrictions\n"
                      "whatsoever. You may copy it, give it away or re-use it under the terms\n"
                      "of the Project Gutenberg License included with this ebook or online\n"
                      "at www.gutenberg.org. If you are not located in the United States,\n"
                      "you will have to check the laws of the country where you are located\n"
                      "before using this eBook.";

    dataHandler.processText(str, "file1");

    Search search(filters, storage);
    std::string find = "parts of the world";
    auto res = search.search(find);
    Search::printSearchResults(res);

    for (auto f : filters) delete f;
    return 0;
}
