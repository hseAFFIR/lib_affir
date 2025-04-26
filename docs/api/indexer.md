# Indexer
## Table of Contents
<!-- TOC -->
* [FileStorage](#filestorage)
    * [Table of Contents](#table-of-contents)
    * [Overview](#overview)
    * [How to use](#how-to-use)
<!-- TOC -->
## Overview
A module for converting single tokens into tokens with grouped data, storing them temporarily and transmitting them for further recording.
Under the hood, input tokens are grouped and stored in a buffer.
The indexing module adds to the existing data in the buffer from the new token, when the buffer is full, it gives the data further down the pipline.

The `bufferSize` specifies the size of the buffer, when filled it will transfer data further down the pipline.
In `indStor` it is passed `indexStorage`.

## How to use
After all tokens have been processed, call `saveTo()`
### To add new token
```c++
#include "affir/storages/multiFileIndexStorage.h"
#include "affir/indexer/indexer.h"

using namespace affir;

...

MultiFileIndexStorage storage;
Token token("example", 100, 10, 1)

Indexer indexer(75, storage);

indexer.addToken(token);

// Remember to manually save the tokens from the buffer after all tokens have been processed
indexer.saveTo() 

...

```

### To get token positions by token name
```c++
#include "affir/storages/multiFileIndexStorage.h"
#include "affir/indexer/indexer.h"

using namespace affir;

...

MultiFileIndexStorage storage;

Indexer indexer(75, storage);

// get BigToken by token name
BigToken resultBt = indexer.getTokenInfo("example");

...

```