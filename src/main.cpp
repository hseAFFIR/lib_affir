#include <iostream>
#include "storages/fileStorage.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::string str = "1234,5687,(),1234,4567,()";
    std::string str2 = "asdf,1234,asdf,ery,()1234";
    auto *fileStorage = new FileStorage("MyFile", str.size());
    auto *fileStorage2 = new FileStorage("MyFile", str2.size());
    fileStorage2->write(str2);
    fileStorage->write(str);
    fileStorage->close();
    fileStorage2->close();
    return 0;
}