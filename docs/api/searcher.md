# Search
## Table of Contents
<!-- TOC -->
* [Search](#search)
    * [Table of Contents](#table-of-contents)
    * [Overview](#overview)
    * [How to use](#how-to-use)
        * [Searching for single words](#searching-for-single-words)
        * [Searching for exact phrases](#searching-for-exact-phrases)
    * [Notes](#notes)
<!-- TOC -->

## Overview
A module for searching indexed tokens and phrases across multiple files. It provides:
- Single word search with positional information
- Exact phrase search (consecutive words)
- Results with file IDs and word positions

The search operates on pre-indexed data through `Indexer` and `MultiFileIndexStorage`.

## How to use

### Initialization
```cpp
#include "storages/multiFileIndexStorage.h"
#include "indexer/indexer.h"
#include "search/search.h"

MultiFileIndexStorage storage;
Indexer indexer(1024, storage);  // Buffer size 1024
PhraseSearcher searcher(storage, indexer);
```

### Searching for single words
```cpp
// Search for all occurrences of a word
auto wordResults = searcher.searchWord("example");

// Process results
for (const auto& match : wordResults) {
    std::cout << "Found '" << match.word 
              << "' in file " << match.fileId
              << " at position " << match.absolutePos
              << " (word #" << match.wordSequencePos << ")\n";
}
```

### Searching for exact phrases
```cpp
// Search for exact phrase (words must appear consecutively)
auto phraseResults = searcher.searchPhrase("quick brown fox");

// Process results
for (const auto& phrase : phraseResults) {
    std::cout << "Found phrase in file " << phrase.fileId << ":\n";
    for (const auto& word : phrase.words) {
        std::cout << "- " << word.word 
                  << " at position " << word.absolutePos << "\n";
    }
}
```

## Notes
Ensure all tokens are indexed and saved before searching:
```cpp
indexer.saveTo();
storage.saveMetadata();
```

Phrase search requires:
1. All words to exist in the same file
2. Words to appear in the exact given order
3. No gaps between words