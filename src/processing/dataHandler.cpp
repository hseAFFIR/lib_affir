#include "dataHandler.h"
#include "../storages/fileStorage.h"
#include "../indexer/indexer.h"
#include <algorithm>

DataHandler::DataHandler(const std::vector<Base*> &filters, const size_t buffer)
    : filters(filters)
{
    std::sort(this->filters.begin(), this->filters.end(), 
        [](const Base* a, const Base* b) {
            return static_cast<int>(a->getOrder()) < static_cast<int>(b->getOrder());
        });
}

void DataHandler::processText(const std::string &text, const std::string &filename) {
    FileStorage fs(filename, text.size());
    FileId id = fs.getId();
    fs.write(std::string_view(text));
    fs.close();
    Tokenizer tk(filters);
    Indexer ind(buffer);
    tk.tokenize(text, [this, &filename, &ind](Token token) {
        ind.addToken(token);
    });
}   