#include <iostream>
#include "indexer/indexer.h"
#include "storages/fileStorage.h"
#include "models/token.h"

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




//    std::string str = "1234,5687,(),1234,4567,()";
//    std::string str2 = "asdf,1234,asdf,ery,()1234";
//    auto *fileStorage = new FileStorage("MyFile", str.size());
//    auto *fileStorage2 = new FileStorage("MyFile", str2.size());
//    fileStorage2->write(str2);
//    fileStorage->write(str);
//    fileStorage->close();
//    fileStorage2->close();

    auto *fileStorage3 = new FileStorage(1);

    std::vector<char> buffer;
    while (!fileStorage3->isEnd()) {
        fileStorage3->read(buffer);
        std::cout << buffer.data() << std::endl;
    }
    FileStorage::saveStorageMeta();
    // Token token("Hello",1,2,1);
    // std::cout<<token<<std::endl;
    // Token token2("Hello",1,2,1);
    // if (token==token2) {
    //     std::cout<<"YES"<<std::endl;
    // }
    // else {
    //     std::cout<<"NO"<<std::endl;
    //
    // }
    return 0;
}