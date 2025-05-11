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

The tokenizer supports two processing modes:

1. **`NATIVE_POSES`** - preserves original token positions
2. **`CLEAR_POSES`** - provides normalized positioning
## How to use

### Tokenizing raw text
Example usage of `Tokenizer` without filtering:
```cpp
### Basic tokenization
```cpp
#include "affir/tokenizer/tokenizer.h"
#include <iostream>

using namespace affir;

int main() {
    // Initialize with NATIVE_POSES mode and no filters
    Tokenizer tokenizer(TokenizerMode::NATIVE_POSES);
    std::string text = "Sample text with <b>HTML</b> tags";
    
    // Process the text
    tokenizer.tokenize(text);
    
    // Iterate through tokens
    while(tokenizer.hasNext()) {
        Token token = tokenizer.next();
        std::cout << "Token: " << token.getBody() 
                  << " | Position: " << token.getPos() << std::endl;
    }
    
    return 0;
}
```

### Tokenizing with filters
To use filtering, pass a list of filters to the `Tokenizer` constructor:
```cpp
#include <vector>
#include <iostream>
#include "affir/tokenizer/filters/filters.h"
#include "affir/tokenizer/tokenizer.h"
#include "affir/models/token.h"

using namespace affir;

int main() {
    // Create filter chain
    std::vector<Base*> filters = {
        new Lowercaser(),    // Convert to lowercase
        new Htmler(),        // Remove HTML tags
        new Punctuator()     // Remove punctuation
    };

    // Initialize with CLEAR_POSES mode and filters
    Tokenizer tokenizer(TokenizerMode::CLEAR_POSES, filters);
    std::string text = "Filtered <i>text</i> with UPPERCASE and punctuation!";
    
    // Process the text
    tokenizer.tokenize(text);
    
    // Iterate through filtered tokens
    while(tokenizer.hasNext()) {
        Token token = tokenizer.next();
        // Process filtered tokens
    }

    // Cleanup filters
    for(auto filter : filters) {
        delete filter;
    }
    
    return 0;
}
```
