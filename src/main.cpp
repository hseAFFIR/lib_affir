#include <iostream>
#include "tokenizer/filters/filters.h"
#include "processing/dataHandler.h"
#include <vector>
#include <cassert>
#include <fstream>
#include "logger/logger.h"
#include "storages/indexes/multi/multiFileIndexStorage.h"

#include "processing/encodingHandler/encodingHandler.h"

int main(){
    // Htmler html();
    Htmler* html = new Htmler();
    Lowercaser* lc = new Lowercaser(); 
    StopWords* sw = new StopWords();
    Punctuator* pc = new Punctuator();
    StemFilter* sf = new StemFilter();
    // Punctuator pc();
    // StemFilter sf();
    // StopWords sw();
    std::vector<Base*> filters = {new Lowercaser(), new Htmler(), new Punctuator(), 
        new StopWords(), new StemFilter()};
    DataHandler dh(filters,100);
    std::string text = "Привет, мир!";

    std::string testtext = "漢";
    EncodingHandler encodingHandler;
    encodingHandler.initConsole();
    std::string utf8Text = encodingHandler.processInput(testtext);
    std::cout << utf8Text << "\n";
    utf8Text = lc->process(utf8Text);
    std::cout << utf8Text;
    
    std::string filename = "test_output.txt";
    dh.processText(text, filename);
}