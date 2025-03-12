#include <iostream>
#include "storages/fileStorage.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    auto *fileStorage = new FileStorage("My_file", 4);
//    fileStorage->write("1234");
    fileStorage->read();
    fileStorage->close();
    return 0;
}