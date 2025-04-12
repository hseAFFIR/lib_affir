#include "dataHandler.h"
#include "../storages/files/fileStorage.h"
#include "../indexer/indexer.h"
#include "../logger/logger.h"
#include <fstream>
#include <algorithm>

DataHandler::DataHandler(const std::vector<Base*> &filters, const size_t buffer, IIndexStorage &indStor)
    : filters(filters), indexStorage(indStor), buffer(buffer)
{
    Logger::info("DataHandler", "DataHandler module initialized");
    std::sort(this->filters.begin(), this->filters.end(),
        [](const Base* a, const Base* b) {
            return a->getOrder() < b->getOrder();
        });
}

void DataHandler::processText(const std::string &text, const std::string &filename) {
    FileStorage fileStorage(filename, text.size());

    FileId fileId = fileStorage.getId();
    fileStorage.write(text);
    FileStorage::saveStorageMeta();

    Tokenizer tokenizer(filters);
    Indexer indexer(buffer, indexStorage);

    tokenizer.tokenize(text, [&indexer](Token token) {
        Logger::debug("dataHandler::processText","Token: {} | Pos: {}", token.body, token.info.pos);
        indexer.addToken(token);
    }, fileId);

    indexer.saveTo();

    indexStorage.saveStorageMeta();
}   