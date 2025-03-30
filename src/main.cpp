#include <iostream>
#include "tokenizer/filters/filters.h"
#include "processing/dataHandler.h"
#include <vector>
#include <fstream>

int main(){

    std::vector<Base*> filters = {new Lowercaser(), new Htmler(), new Punctuator(),
        new StopWords(), new StemFilter()};

    Tokenizer tokenizer(filters);

    std::string text = "Hello, this is a <b>test</b> text with HTML and some stopwords.";

    FileId fileId = 1;

    tokenizer.tokenizeFiltered(text, fileId, [](Token token) {
        std::cout << "Token: " << token.getBody() << " | Pos: " << token.getPos() << std::endl;
    });

    for (auto filter : filters) {
        delete filter;
    }

    return 0;
}