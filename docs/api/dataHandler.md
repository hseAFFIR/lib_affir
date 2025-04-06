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
and the [Indexer](indexer.md) that itself uses [Multifile Index Storage](multiFileIndexStorage.md) to save the index.

## How to use

### To process data
```c++
#include "tokenizer/filters/filters.h"
#include "processing/dataHandler.h"
#include <vector>

...

int main(){
    // Create a vector with all the needed filters
    std::vector<Base*> filters = {new Lowercaser(), new Htmler(), new Punctuator(), 
        new StopWords(), new StemFilter()};
    
    // Initialize DataHandler with filters and bufferSize, used by Indexer
    DataHandler dh(filters, 100);

    std::string text = "This is a test text. <b>Hello<\b> World!";
    
    // File name used by fileStorage to name your specific input string in its metadata
    std::string filename = "test_output";
    
    dh.processText(text, filename);
    for (auto filter : filters) {
        delete filter;
    }
}
```