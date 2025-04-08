#include <iostream>
#include "tokenizer/filters/filters.h"
#include "processing/dataHandler.h"
#include "storages/indexes/multi/multiFileIndexStorage.h"
#include <vector>
#include <cassert>
#include <fstream>
#include "processing/encodingHandler/encodingHandler.h"
#include "tokenizer/filters/russianPorterStemmer.h"


int main(){
    MultiFileIndexStorage storage;
    std::vector<Base*> filters = {new Lowercaser(), new Htmler(), new Punctuator(), 
        new StopWords(), new StemFilter()};
    DataHandler dh(filters,100, storage);
    std::string text = "Привет, мир!";
    RussianPorterStemmer rps;
    
    EncodingHandler encodingHandler;
    encodingHandler.initConsole();
    
    std::string testtexttt = "селением";
    std::string utf8test = encodingHandler.processInput(testtexttt);
    std::cout << rps.process(utf8test);
    std::string testtext = "漢";
    std::string utf8Text = encodingHandler.processInput(testtext);
    std::cout << utf8Text << "\n";
    std::cout << utf8Text;
    
    std::string filename = "test_output.txt";
    dh.processText(text, filename);
}