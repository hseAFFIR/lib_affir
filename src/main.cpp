#include <iostream>
#include "indexer/indexer.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    auto *indexer1 = new Indexer(100);

    Token token1;
    token1.body = "apple";
    token1.fileId = 1;
    token1.pos = 100;
    token1.wordPos = 1;

    Token token2;
    token2.body = "banana";
    token2.fileId = 2;
    token2.pos = 200;
    token2.wordPos = 2;

    Token token3;
    token3.body = "apple2";
    token3.fileId = 1;
    token3.pos = 300;
    token3.wordPos = 3;


    std::cout << "Adding tokens..." << std::endl;
    for (int i = 0; i < 2; ++i) {
        token1.pos++;
        token2.pos++;
        token3.pos++;

        token1.wordPos++;
        token2.wordPos++;
        token3.wordPos++;

        indexer1->addToken(token1);
        indexer1->addToken(token2);
        indexer1->addToken(token3);
    }

    std::cout << "\nChecking buffer after overflow:" << std::endl;
    auto buffer = indexer1->getBuffer();
    std::cout << "Current buffer size: " << buffer.size() << " entries" << std::endl;




    return 0;
}