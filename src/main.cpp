#include <iostream>
#include "tokenizer/filters/filters.h"
#include "processing/dataHandler.h"
#include <vector>
#include <cassert>
#include <fstream>

int main(){
    // Htmler html();
    // Lowercaser lc();
    // Punctuator pc();
    // StemFilter sf();
    // StopWords sw();
    std::vector<Base*> filters = {new Lowercaser(), new Htmler(), new Punctuator(), 
        new StopWords(), new StemFilter()};
    DataHandler dh(filters,100);
    std::string text = "This is a test text. <b>Hello<\b> World!";
    std::string filename = "test_output.txt";
    dh.processText(text, filename);
}