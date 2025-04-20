#include "dataHandler.h"
#include "../logger/logger.h"
#include <fstream>
#include <algorithm>

DataHandler::DataHandler(TokenizerMode tokenizerMode, const std::vector<Base*> &filters, const size_t buffer, IIndexStorage &indStor)
    : filters(filters), indexStorage(indStor)
{
    Logger::info("DataHandler", "DataHandler module initialized");
    tokenizer = new Tokenizer(tokenizerMode, filters);
    indexer = new Indexer(buffer, indexStorage);
}

void DataHandler::processText(std::string &text, FileId fileId) {
    tokenizer->tokenize(text, fileId);

    while(tokenizer->hasNext()) {
        Token token = tokenizer->next();
        Logger::debug("DataHandler (processText)", "Token: {} | Pos: {}", token.body, token.info.pos);
        indexer->addToken(token);
    }
}

void DataHandler::flush() {
    indexer->saveTo();
    indexStorage.saveStorageMeta();
}

DataHandler::~DataHandler() {
    flush();
    delete tokenizer;
    delete indexer;
}
