# Tokenizer

## Table of Contents
<!-- TOC -->
* [Tokenizer](#tokenizer)
    * [Overview](#overview)
    * [How to use](#how-to-use)
        * [Tokenizing raw text](#tokenizing-raw-text)
        * [Tokenizing with filters](#tokenizing-with-filters)
<!-- TOC -->

## Overview
`Tokenizer` is a module for splitting text into tokens. It supports basic tokenization and allows adding filters for text preprocessing.

Filters may include:
- Converting text to lowercase
- Removing HTML tags
- Removing punctuation
- Filtering out stop words
- Stemming

## How to use

### Tokenizing raw text
Example usage of `Tokenizer` without filtering:
```cpp
#include "Tokenizer.h"
#include <iostream>

int main() {
    Tokenizer tokenizer({});
    
    std::string text = "Hello, this is a <b>test</b> message!";
    FileId fileId = 1;

    tokenizer.tokenizeRaw(text, fileId, [](Token token) {
        std::cout << "Token: " << token.getBody() << " | Pos: " << token.getPos() << std::endl;
    });

    return 0;
}
```

### Tokenizing with filters
To use filtering, pass a list of filters to the `Tokenizer` constructor:
```cpp
#include "tokenizer/filters/filters.h"
#include <vector>
#include <iostream>

int main() {
    std::vector<Base*> filters = {new Lowercaser(), new Htmler(), new Punctuator(),
                                  new StopWords(), new StemFilter()};

    Tokenizer tokenizer(filters);
    std::string text = "Hello, this is a <b>test</b> text with HTML and some stopwords.";
    FileId fileId = 1;

    tokenizer.tokenizeFiltered(text, fileId, [](Token token) {
        std::cout << "Token: " << token.getBody() << " | Pos: " << token.getPos() << std::endl;
    });

    for (auto filter : filters) {
        delete filter;
    }

    return 0;
}
```
