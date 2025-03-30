#ifndef TOKENIZER_PIPELINE_H
#define TOKENIZER_PIPELINE_H

#include <vector>
#include <regex>
#include <functional>
#include "filters/base.h"
#include "../models/Token.h"

class TokenizerPipeline {
public:
    explicit TokenizerPipeline(std::vector<Base*> filters);

    std::vector<Token> run(const std::string &text, FileId fileId);

    void tokenize(const std::string &text, FileId fileId, std::function<void(Token)> callback);

    void addFilter(Base* filter);

private:
    std::vector<Base*> filters;
    std::regex htmlPattern;

    std::vector<Token> tokenize(const std::string &text, FileId fileId);

    std::string applyFilters(const std::string &token);
};

#endif // TOKENIZER_PIPELINE_H
