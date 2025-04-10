#include "dataHandler.h"
#include "../storages/files/fileStorage.h"
#include "../indexer/indexer.h"
#include <fstream>
#include "../logger/logger.h"
#include <algorithm>

DataHandler::DataHandler(const std::vector<Base*> &filters, const size_t buffer, IIndexStorage &indStor)
    : filters(filters), indexStorage(indStor), buffer(buffer)
{
    Logger::info("Indexer", "Indexer module initialized");
    std::sort(this->filters.begin(), this->filters.end(), 
        [](const Base* a, const Base* b) {
            return static_cast<int>(a->getOrder()) < static_cast<int>(b->getOrder());
        });

    encodingHandler.initConsole();
}

DataHandler::DataHandler(const size_t buffer, IIndexStorage &indStor)
    : filters({}), indexStorage(indStor), buffer(buffer) {
    Logger::info("Indexer", "Indexer module initialized");
    }

void DataHandler::processText(const std::string &text, const std::string &filename) {
    std::string utf8Text = encodingHandler.processInput(text);
    FileStorage fs(filename, utf8Text.size());
    FileId id = fs.getId();
    fs.write(utf8Text);
    Tokenizer tk(filters);
    Indexer ind(buffer, indexStorage);
    if(!filters.empty()) {
        tk.tokenizeFiltered(utf8Text, id, [&ind](Token token) {
            Logger::debug("dataHandler::processText","Token: {}", token.getBody());
            ind.addToken(token);
        });
    }
    else {
        tk.tokenizeRaw(utf8Text, id, [&ind](Token token) {
            Logger::debug("dataHandler::processText","Token: {}", token.getBody());
            ind.addToken(token);
        });
    }
    FileStorage::saveStorageMeta();
    ind.saveTo();
    indexStorage.saveStorageMeta();
}   