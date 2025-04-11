#include <vector>

#include "tokenizer/filters/filters.h"
#include "processing/dataHandler.h"
#include "storages/indexes/multi/multiFileIndexStorage.h"
#include "storages/indexes/single/singleIndexStorage.h"
#include "logger/logger.h"

int main(){
    Logger::init("logs/log.txt");
    Logger::info("Main", "Application started");
    return 0;
}
