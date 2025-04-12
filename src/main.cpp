#include <vector>

#include "tokenizer/filters/filters.h"
#include "processing/dataHandler.h"
#include "storages/indexes/multi/multiFileIndexStorage.h"
#include "storages/indexes/single/singleIndexStorage.h"
#include "logger/logger.h"

int main(){
    setlocale(LC_ALL, "Russian");

    Logger::init("logs/log.txt");
    Logger::info("Main", "Application started");

    std::string a = "Всем привёт!";
    for (char ch : a) {
        std::wcout << ch << "\n";
    }

    return 0;
}
