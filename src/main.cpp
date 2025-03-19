#include <iostream>
#include "indexer/indexer.h"
#include "storages/fileStorage.h"
#include "models/token.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    auto *indexer1 = new Indexer(100);

    auto *token1 = new Token("apple", 1, 200, 1);
    auto *token2 = new Token("banana", 2, 300, 2);
    auto *token3 = new Token("apple2", 1, 400, 3);


    std::cout << "Adding tokens..." << std::endl;
    for (int i = 0; i < 2; ++i) {
//        token1.pos++;
//        token2.pos++;
//        token3.pos++;
//
//        token1.wordPos++;
//        token2.wordPos++;
//        token3.wordPos++;

        indexer1->addToken(*token1);
        indexer1->addToken(*token2);
        indexer1->addToken(*token3);
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

//    auto *fileStorage3 = new FileStorage(1);
//
//    std::vector<char> buffer;
//    while (!fileStorage3->isEnd()) {
//        fileStorage3->read(buffer);
//        std::cout << buffer.data() << std::endl;
//    }
//    FileStorage::saveStorageMeta();
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