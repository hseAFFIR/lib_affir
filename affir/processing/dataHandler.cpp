#include <fstream>
#include <algorithm>
#include "dataHandler.h"
#include "../logger/logger.h"

using namespace affir;

DataHandler::DataHandler(TokenizerMode tokenizerMode, const std::vector<Base *> &filters, const size_t buffer,
                         IIndexStorage &indStor)
        : filters(filters), indexStorage(indStor) {
    LOG_INFO(Logger::logger, "DataHandler module init");
    tokenizer = new Tokenizer(tokenizerMode, filters);
    indexer = new Indexer(buffer, indexStorage);
}

void DataHandler::processText(std::string &text, FileId fileId) {
    tokenizer->tokenize(text, fileId);

    while (tokenizer->hasNext()) {
        Token token = tokenizer->next();
        LOG_DEBUG(Logger::logger, "dataHandler (processText)", "Token: {} | Pos: {}", token.body, token.info.pos);
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
