#include "dataHandler.h"
#include "../storages/fileStorage.h"

#include <algorithm>

DataHandler::DataHandler(const std::vector<Base*> &filters)
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
    tk.tokenize(text, [this, &filename](Token token) {
        //Indexer::store_token(token, filename);
        // Дополнительная обработка при необходимости
    });
}   