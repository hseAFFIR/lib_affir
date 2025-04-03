#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <regex>
#include <functional>
#include "filters/base.h"
#include "../models/Token.h"

class tokenizer {
public:
    explicit tokenizer(std::vector<Base*> filters);

    void tokenizeRaw(const std::string &text, FileId fileId, std::function<void(Token)> callback);

    void tokenizeFiltered(const std::string &text, FileId fileId, std::function<void(Token)> callback);

    void addFilter(Base* filter);

private:
    std::vector<Base*> filters;
    std::regex htmlPattern;
    std::string applyFilters(const std::string &token);
};

#endif // TOKENIZER_H