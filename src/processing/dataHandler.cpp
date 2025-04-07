#include "dataHandler.h"
#include "../storages/files/fileStorage.h"
#include "../indexer/indexer.h"
#include <fstream>
#include <algorithm>

DataHandler::DataHandler(const std::vector<Base*> &filters, const size_t buffer, IIndexStorage &indStor)
    : filters(filters), indexStorage(indStor)
{
    std::sort(this->filters.begin(), this->filters.end(), 
        [](const Base* a, const Base* b) {
            return static_cast<int>(a->getOrder()) < static_cast<int>(b->getOrder());
        });
}

DataHandler::DataHandler(const size_t buffer, IIndexStorage &indStor)
    : filters({}), indexStorage(indStor) {}

void DataHandler::processText(const std::string &text, const std::string &filename) {
    FileStorage fs(filename, text.size());
    FileId id = fs.getId();
    fs.write(text);
    Tokenizer tk(filters);
    Indexer ind(buffer, indexStorage);
    if(!filters.empty()) {
        tk.tokenizeFiltered(text, id, [&ind](Token token) {
            std::cout << "Token: " << token.getBody() << " | Pos: " << token.getPos() << std::endl;
            ind.addToken(token);
        });
    }
    else {
        tk.tokenizeRaw(text, id, [&ind](Token token) {
            std::cout << "Token: " << token.getBody() << " | Pos: " << token.getPos() << std::endl;
            ind.addToken(token);
        });
    }
    FileStorage::saveStorageMeta();
    ind.saveTo();
    //indexStorage.saveMetadata();
}   