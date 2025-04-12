#include "storages/indexes/multi/multiFileIndexStorage.h"
#include "indexer/indexer.h"
#include "searcher/search.h"
#include "logger/logger.h"


int main() {
    Logger::init("logs/log.txt");
    Logger::info("Main", "Application started");

    MultiFileIndexStorage storage;
    Indexer indexer(1024, storage);
    Search searcher(indexer);

    // Search for a single word
    auto results = searcher.search("fox");

    // Print results using built-in function
    searcher.printSearchResults(results);

    return 0;
}
