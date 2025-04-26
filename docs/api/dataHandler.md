# DataHandler
## Table of Contents
<!-- TOC -->
* [DataHandler](#datahandler)
  * [Table of Contents](#table-of-contents)
  * [Overview](#overview)
  * [How to use](#how-to-use)
<!-- TOC -->
## Overview
A module that is used to fully process the initial input text from a string. 
It automatically passes the data through the [File Storage](fileStorage.md), [Tokenizer](tokenizer.md) with the needed filters,
and the [Indexer](indexer.md) that itself uses [Multifile Index Storage](multiFileIndexStorage.md) or
Singlefile Index Storage to save the index, passed as a parameter by the user.

Can be used with or without Tokenizer filters specified.

## How to use
### Initialization
Class takes 3 parameters:
1. Engine focus:
   It is required because of the specific of UTF-8 encoding. English symbol size equals 1 byte, whereas a russian one is 2 bytes.
  - `TokenizerMode::NATIVE_POSES`\
    Focus on storaging 'clear' positions, that means **1 character = 1 position**.
    However, this leads to **incorrect display of search results in the context**.
  - `TokenizerMode::CLEAR_CONTEXT`\
    On the contrary, it saves the positions of tokens according
    to their native encoding (mostly UTF-8), i.e. **1 character ≠ 1 position**,
    however, it allows you to output the **correct context for search results**.
2. Vector of pointers to filters
  - You can find them in [filters.h](../../affir/tokenizer/filters/filters.h)
3. Buffer size (_which will be used by the [Indexer](indexer.md)_)
4. Reference to [IndexStorage](indexStorage.md)
### To process data
```c++
#include <vector>
#include "affir/tokenizer/filters/filters.h"
#include "affir/processing/dataHandler.h"
#include "affir/storages/indexes/multi/multiFileIndexStorage.h"

using namespace affir;
...

int main(){
    // Create a vector with all the needed filters
    std::vector<Base*> filters = {new Lowercaser(), new Htmler(), new Punctuator(), 
        new StopWords(), new StemFilter()};

    // Initialize MFIS (or SFIS)
    MultiFileIndexStorage storage;
    
    // Initialize DataHandler with TokenizerMode, filters, bufferSize and indexStorage, used by Indexer
    DataHandler dh(TokenizerMode::CLEAR_POSES, filters, 100, storage);

    std::string text = "This is a test text. <b>Hello<\b> World!";
    
    // File name used by fileStorage to name your specific input string in its metadata
    std::string filename = "test_output";
    
    dh.processText(text, filename);
    for (auto filter : filters) {
        delete filter;
    }
}
```