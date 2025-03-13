#include <iostream>
#include "indexer/indexer.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    auto *indexer1 = new Indexer(100);

    Token token1;
    token1.body = "1";
    token1.fileId = 1;
    token1.pos = 1;
    token1.wordPos = 1;

    for (int i=0; i < 100; i++) {
        std::cout<<i<<std::endl;
        indexer1->addToken(token1);
    }


    return 0;
}