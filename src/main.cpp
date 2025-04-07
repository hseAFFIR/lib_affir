#include <iostream>
#include "tokenizer/filters/filters.h"
#include "processing/dataHandler.h"
#include "storages/indexes/multi/multiFileIndexStorage.h"
#include <vector>
#include <cassert>
#include <fstream>

int main(){
    std::vector<Base*> filters = {new Lowercaser(), new Htmler(), new Punctuator(), 
        new StopWords(), new StemFilter()};
    MultiFileIndexStorage storage;
    // DataHandler dh(filters, 100, storage);
    DataHandler dh(100, storage);
    std::string text = "This is a test text. <b>Hello<\b> World!";
    std::string filename = "test_output.txt";
    dh.processText(text, filename);
    for (auto filter : filters) {
        delete filter;
    }
}