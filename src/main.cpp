#include <iostream>
#include "storages/fileStorage.h"
#include "models/token.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
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