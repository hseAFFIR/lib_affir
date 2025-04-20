#include "dataHandler.h"
#include "../storages/files/fileStorage.h"
#include "../logger/logger.h"
#include <fstream>
#include <algorithm>

DataHandler::DataHandler(const std::vector<Base*> &filters, const size_t buffer, IIndexStorage &indStor)
    : filters(filters), indexStorage(indStor)
{
    logger = Logger::GetRootLogger();
    LOG_INFO(logger, "DataHandler module init");

    std::sort(this->filters.begin(), this->filters.end(),
        [](const Base* a, const Base* b) {
            return a->getOrder() < b->getOrder();
        });
    tokenizer = new Tokenizer(filters);
    indexer = new Indexer(buffer, indexStorage);
}

void DataHandler::processText(std::string &text, const std::string &filename) {
    FileStorage fileStorage(filename, text.size());

    FileId fileId = fileStorage.getId();
    fileStorage.write(text);
    FileStorage::saveStorageMeta();

    tokenizer->tokenize(text, fileId);

    while(tokenizer->hasNext()) {
        Token token = tokenizer->next();
        LOG_DEBUG( logger,"dataHandler::processText", "Token: {} | Pos: {}", token.body, token.info.pos);
        indexer->addToken(token);
    }

    indexer->saveTo();

    indexStorage.saveStorageMeta();
}

DataHandler::~DataHandler() {
    delete tokenizer;
    delete indexer;
}
