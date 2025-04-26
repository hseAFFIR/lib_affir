# Searcher
## Table of Contents
<!-- TOC -->
* [Searcher](#searcher)
    * [Table of Contents](#table-of-contents)
    * [Overview](#overview)
    * [How to use](#how-to-use)
        * [Initialization](#initialization)
        * [Searching for single words](#searching-for-single-words)
        * [Searching for exact phrases](#searching-for-exact-phrases)
    * [Class Structure](#class-structure)
    * [Notes](#notes)
<!-- TOC -->

## Overview
The `Searcher` class is responsible for performing search operations on indexed data. It provides:
- Single word search with positional information
- Exact phrase search (consecutive words)
- Results with file IDs and word positions

The search operates on pre-indexed data through `Indexer` and `MultiFileIndexStorage`.

## How to use

### Initialization
```cpp
#include "affir/storages/multiFileIndexStorage.h"
#include "affir/indexer/indexer.h"
#include "affir/search/search.h"

using namespace affir;

MultiFileIndexStorage storage;
Indexer indexer;
Searcher searcher(indexer);
```

### Searching for single words
```cpp
// Search for a single word
auto results = searcher.search("fox");

// Print results using built-in function
std::cout << results.toString();
```

### Searching for exact phrases
```cpp
// Search for exact phrase (words must appear consecutively)
auto results = searcher.search("quick brown fox");

// Print results using built-in function
std::cout << results.toString();
```

## Class Structure

### SearchResult Structure
```cpp
class SearchResult {
    std::string query;       ///< Tokenized query
    size_t queryOriginalSize{};
    PosMap posMap;           ///< Position map in files
    ...
};
```

### Public Methods
- Constructor `Searcher(const MultiFileIndexStorage& storage, Indexer& indexer)` 
- Main Search method `std::vector<SearchResult> search(const std::string& query) const` 
- Print Results `void printSearchResults(const std::vector<SearchResult>& results) const`

### Private Methods
- Word Adjacency Check `bool areWordsAdjacent(FileId fileId, const std::vector<TokenInfo>& firstInfos, const std::vector<TokenInfo>& secondInfos) const`
- Single Word Search`std::vector<SearchResult> searchSingleWord(const std::string& word) const`
- Phrase Search `std::vector<SearchResult> searchPhrase(const std::vector<std::string>& words) const` 

## Notes
Ensure all tokens are indexed before searching:
```cpp
indexer.saveTo();
storage.saveMetadata();
```

Phrase search requires:
1. All words to exist in the same file.
2. Words to appear in the exact given order.
3. **No gaps between words.**

The class maintains references to storage and indexer components for efficient search operations.